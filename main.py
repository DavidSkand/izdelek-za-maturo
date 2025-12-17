import cv2
from pyapriltags import Detector
import time

camera = cv2.VideoCapture(1) 

at_detector = Detector()


#preveri če kamera deluje
if not camera.isOpened:  
    print('cannot open camera')
    exit()

start_time = time.time()  # Start the timer

while(camera.isOpened()):
    ret, frame = camera.read()       #pridobi ret, Width,Height
    if ret == False:    
        print('something went wrong')
        break
      

    if ret == True:
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  
        tags = at_detector.detect(gray)   #lista tags

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

        elapsed_time = time.time() - start_time  
        fps = 1 / elapsed_time  # FPS = 1 / Time per frame

        cv2.putText(frame, f"FPS: {fps:.2f}", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 0), 2)

        start_time = time.time()


        cv2.imshow('kamera', frame)
        
        if cv2.waitKey(25) & 0xFF == ord('q'):
            break


camera.release()

cv2.destroyAllWindows()
