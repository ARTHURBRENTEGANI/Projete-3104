import cv2
from ultralytics import YOLO

model = YOLO("C:\\Users\\simba\\Downloads\\bestp2.pt") 

print("Iniciando captura da câmera")

camera = cv2.VideoCapture(0,cv2.CAP_DSHOW)
print("camera concluida")

if not camera.isOpened():
    print("Erro ao abrir a câmera.")
else:
    print("Câmera aberta com sucesso.")

while True:
    status, frame = camera.read()
    if not status:
        print("Não foi possível capturar o frame.")
        break
    results = model(frame) # Aplicar o modelo YOLOv8 ao frame
    annotated_frame = results[0].plot()  # Resultados anotados no frame
    cv2.imshow("Detecções YOLOv8", annotated_frame)     # Exibir o frame com as detecções
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Libera os recursos da câmera
camera.release()
cv2.destroyAllWindows()

print("Processo concluído")
