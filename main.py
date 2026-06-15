import cv2
from pyapriltags import Detector
import time
import serial

camera = cv2.VideoCapture(0) 

at_detector = Detector()

start_time = time.time()  # Start the timer

arduino = serial.Serial(port='COM3',   baudrate=115200, timeout=.1)

last_x, last_y = 320, 240

last_seen = time.time()

timeout = 0.5 

last_sent_x = -1
last_sent_y = -1

time.sleep(2)  
last_send = 0
send_period = 0.03  # 33 Hz

#preveri če kamera deluje
if not camera.isOpened():  
    print('cannot open camera')
    exit()


while(camera.isOpened()):
    ret, frame = camera.read()       #pridobi ret, Width,Height
    if ret == False:    
        print('something went wrong')
        break
      

    if ret == True:
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  
        tags = at_detector.detect(gray)   #lista tagsx
        now = time.time()

        if len(tags) > 0:
            # choose one tag (first one)
            tag = tags[0]
            center = tag.center
            last_x, last_y = int(center[0]), int(center[1])


        #vzame listo in kliče vsak tag posebaj
        for tag in tags: 
            center = tag.center
            tagid = tag.tag_id
            width, height = 80, 80
            top_left = (int(center[0] - width/2), int(center[1] - height/2))
            bottom_right = (int(center[0] + width/2), int(center[1] + height/2))
            cv2.rectangle(frame, top_left,  bottom_right, (255,0,0), 2)
            text_position = (int(center[0] - width / 4), int(center[1] - height / 4))
            cv2.putText(frame, f"ID: {tagid}", text_position, cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0,0,255), 2 )
            xy_position = f"X: {int(center[0])}, Y: {int(center[1])}"
            cv2.putText(frame, xy_position, (bottom_right[0], bottom_right[1] + 20),  cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)

        now2 = time.time()
        if now2 - last_send >= send_period:
           
           if last_x >= 0 and last_y >= 0:
               if abs(last_x - last_sent_x) > 2 or abs(last_y - last_sent_y) > 2:
                   arduino.write(f"{last_x},{last_y}\n".encode())
                   
                   last_sent_x = last_x
                   last_sent_y = last_y

           # Keep the buffer clear but don't print anything
           if arduino.in_waiting > 0:
               arduino.read_all() 
            
           last_send = now2


        # Izračun FPS
        loop_time = time.time() - start_time
        if loop_time > 0:
            fps = 1 / loop_time
            cv2.putText(frame, f"FPS: {fps:.2f}", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 0), 2)
        
        start_time = time.time()


        cv2.imshow('kamera', frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        
camera.release()
cv2.destroyAllWindows()
arduino.close()  



