import cv2
from ultralytics import YOLO

# Carregar o modelo treinado (.pt)
model = YOLO("C:\\Users\\simba\\Downloads\\best.pt")

# Inicializar a câmera (a linha foi adaptada para incluir o método CAP_DSHOW)
print("teste")
camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)
print("foi")

# Verificar se a câmera foi aberta corretamente
if not camera.isOpened():
    print("Erro ao abrir a câmera.")

while True:
    # Capturar o frame da câmera
    status, frame = camera.read()
    if not status:
        print("Não foi possível capturar o frame.")
        break
    
    # Fazer a predição usando o modelo carregado
    results = model(frame)  # Processar o frame
    
    # Plotar as keypoints no frame capturado
    annotated_frame = results[0].plot()  # Desenhar as keypoints no frame

    # Acessar as detecções
    detections = results[0].boxes
    for detection in detections:
        class_id = int(detection.cls)  # ID da classe
        confidence = detection.conf  # Confiança da detecção
        
        # Verificar a classe e exibir mensagem se for 'person_wrong'
        if class_id == 1 and confidence > 0.5:  # Supondo que 'person_wrong' é a classe 1
            print("deve acender o led amarelo")
    
    # Mostrar o frame com as keypoints
    cv2.imshow("Camera", annotated_frame)

    # Fechar a janela ao pressionar 'q'
    if cv2.waitKey(1) == ord('q'):
        break

# Liberar a câmera e fechar as janelas
camera.release()
cv2.destroyAllWindows()
