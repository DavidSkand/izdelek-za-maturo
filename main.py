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

time.sleep(2)  
last_send = 0
send_period = 0.05  # 20 Hz

#preveri če kamera deluje
if not camera.isOpened():  
    print('cannot open camera')
    exit()


def write_read(x):
    arduino.write(bytes(x,   'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return  data




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
            last_seen = now
        else:
            # no detection: hold last for a bit, then go to center
            if now - last_seen > timeout:
                last_x, last_y = 320, 240


        #vzame listo in kliče vsak tag posebaj
        for tag in tags: 
            center = tag.center
            tagid = tag.tag_id
            width, height = 80, 80
            top_left = (int(center[0] - width/2), int(center[1] - height/2))
            bottom_right = (int(center[0] + width/2), int(center[1] + height/2))
            cv2.rectangle(frame, top_left,  bottom_right, (255,0,0), 2)
            text_position = (int(center[0] - width / 4), int(center[1] - height / 4))
            cv2.putText(frame, f"ID: {tagid}", text_position, cv2.FONT_HERSHEY_SIMPLEX, (1.5), (0,0,255), 2 )
            xy_position = f"X: {int(center[0])}, Y: {int(center[1])}"
            cv2.putText(frame, xy_position, (bottom_right[0], bottom_right[1] + 20),  cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)

        now2 = time.time()
        if now2 - last_send >= send_period:
            arduino.write(f"{last_x},{last_y}\n".encode())
            last_send = now2

        elapsed_time = time.time() - start_time  
        fps = 1 / elapsed_time  # FPS = 1 / Time per frame

        cv2.putText(frame, f"FPS: {fps:.2f}", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 0), 2)

        start_time = time.time()


        cv2.imshow('kamera', frame)
        
        if cv2.waitKey(25) & 0xFF == ord('q'):
            break


camera.release()

cv2.destroyAllWindows()
