import cv2
import time
import requests
from ultralytics import YOLO

# Carregar o modelo treinado (.pt)
model = YOLO("C:\\Users\\simba\\Downloads\\best.pt")

# Inicializar a câmera (a linha foi adaptada para incluir o método CAP_DSHOW)
camera = cv2.VideoCapture(0, cv2.CAP_DSHOW)

# Verificar se a câmera foi aberta corretamente
if not camera.isOpened():
    print("Erro ao abrir a câmera.")

# Configurações do ESP32
esp32_ip = "192.168.4.1"  # IP do ESP32
esp32_port = 600  # Porta do ESP32
esp32_url = "http://192.168.4.1:600/"  # URL para comunicação com o ESP32

# Variáveis para controle do tempo
start_time = None
wrong_pose_duration = 5  # Tempo em segundos para considerar a pessoa "errada" por mais de 5 segundos

while True:
    # Capturar o frame da câmera
    status, frame = camera.read()
    if not status:
        print("Não foi possível capturar o frame.")
        break
    
    # Fazer a predição usando o modelo carregado
    results = model(frame)  # Processar o frame
    
    # Obter as classes detectadas (class_index 1 seria "person_wrong")
    detected_classes = results[0].boxes.cls.tolist()
    
    # Verificar se a classe "person_wrong" (index 1) está presente
    if 1 in detected_classes:
        if start_time is None:
            # Inicia o contador quando a classe "person_wrong" for detectada
            start_time = time.time()
        else:
            # Verifica se a pessoa está na classe errada por mais de 5 segundos
            elapsed_time = time.time() - start_time
            if elapsed_time > wrong_pose_duration:
                # Enviar requisição ao ESP32 para acender o LED amarelo
                try:
                    response = requests.get(esp32_url, params={"led": "amarelo"})
                    print("Pessoa errada por mais de 5 segundos. Alerta enviado ao ESP32.")
                except requests.exceptions.RequestException as e:
                    print(f"Erro ao enviar a requisição para o ESP32: {e}")
                # Reinicia o contador após o envio
                start_time = None
    else:
        # Reseta o contador se a pessoa não estiver na classe errada
        start_time = None

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
