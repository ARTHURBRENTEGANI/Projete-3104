import time
import cv2
import requests
from ultralytics import YOLO

model = YOLO("C:\\Users\\simba\\Downloads\\best.pt")
camera = cv2.VideoCapture(1, cv2.CAP_DSHOW)

esp32_ip = "192.168.4.1"
esp32_port = 600
esp32_url = f"http://{esp32_ip}:{esp32_port}/"

if not camera.isOpened():
    print("Erro ao abrir a câmera.")

framec = 0
frame_p = 20

while True:
    print("frame")
    status, frame = camera.read()
    if not status:
        print("Não foi possível capturar o frame.")
        break

    print("model")
    results = model(frame)

    annotated_frame = results[0].plot()
    cv2.imshow("Camera", annotated_frame)

    print("camera")
    if cv2.waitKey(1) == ord('q'):
        break

    framec += 1

    if framec % frame_p != 0:
        continue

    detections = results[0].boxes
    pessoa_errada_detectada = False

    for detection in detections:
        class_id = int(detection.cls)
        confidence = detection.conf
        print(f"Class ID: {class_id}, Confidence: {confidence}")

        if class_id == 1:
            pessoa_errada_detectada = True
            print("Pessoa errada detectada")
            try:
                print("enviando...")
                response = requests.get(esp32_url, params={"comando": "pessoa_errada"})
                response.close()
                print("Pronto....")
            except Exception as e:
                print(f"Erro ao enviar comando para o ESP32: {e}")
    
    if not pessoa_errada_detectada:
        try:
            print("enviando comando de apagar LED")
            response = requests.get(esp32_url, params={"comando": "apagar_led"})
            response.close()
            print("PRONTO")
        except Exception as e:
            print(f"Erro ao enviar comando para o ESP32: {e}")

    time.sleep(2)
    print("fim for")

camera.release()
cv2.destroyAllWindows()
