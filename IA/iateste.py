import cv2
import requests
from ultralytics import YOLO

# Carregar o modelo treinado (.pt)
model = YOLO("C:\\Users\\simba\\Downloads\\best.pt")

# Inicializar a câmera
print("teste")
camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)
print("foi")

# Configuração do ESP32
esp32_ip = "192.168.4.1"  # IP do ESP32
esp32_port = 600  # Porta do ESP32
esp32_url = "http://192.168.4.1:600/"

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
    
    # Acessar as detecções
    detections = results[0].boxes
    for detection in detections:
        class_id = int(detection.cls)  # ID da classe
        confidence = detection.conf  # Confiança da detecção
        
        # Imprimir o ID da classe e a confiança
        print(f"Class ID: {class_id}, Confidence: {confidence}")
        
        # Enviar mensagem ao ESP32 se a classe for 'person_wrong'
        if class_id == 1 and confidence > 0.7:  # Ajuste o ID da classe conforme necessário
            print("Pessoa errada")
            try:
                # Enviar comando para o ESP32
                requests.get(esp32_url, params={"comando": "pessoa_errada"})
            except Exception as e:
                print(f"Erro ao enviar comando para o ESP32: {e}")
    
    # Plotar as keypoints no frame capturado
    annotated_frame = results[0].plot()  # Desenhar as keypoints no frame
    
    # Mostrar o frame com as keypoints
    cv2.imshow("Camera", annotated_frame)

    # Fechar a janela ao pressionar 'q'
    if cv2.waitKey(1) == ord('q'):
        break

# Liberar a câmera e fechar as janelas
camera.release()
cv2.destroyAllWindows()
