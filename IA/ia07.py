import time
import cv2
import requests
from ultralytics import YOLO

# Carregar o modelo treinado (.pt)
model = YOLO("C:\\Users\\simba\\Downloads\\best.pt")

# Inicializar a câmera
camera = cv2.VideoCapture(1, cv2.CAP_DSHOW)

# Configuração do ESP32
esp32_ip = "192.168.4.1"  # IP do ESP32
esp32_port = 600  # Porta do ESP32
esp32_url = f"http://{esp32_ip}:{esp32_port}/"

# Verificar se a câmera foi aberta corretamente
if not camera.isOpened():
    print("Erro ao abrir a câmera.")

# Inicializar contador de frames fora do loop principal
framec = 0
frame_p = 30  # Definir o intervalo de frames para pular

while True:
    print("frame")
    # Capturar o frame da câmera
    status, frame = camera.read()
    if not status:
        print("Não foi possível capturar o frame.")
        break

    # Fazer a predição usando o modelo carregado
    print("model")
    results = model(frame)  # Processar o frame   

    # Plotar as keypoints no frame capturado
    annotated_frame = results[0].plot()  # Desenhar as keypoints no frame
    
    # Mostrar o frame com as keypoints
    cv2.imshow("Camera", annotated_frame)

    print("camera")
    # Fechar a janela ao pressionar 'q'
    if cv2.waitKey(1) == ord('q'):
        break

    # Incrementar o contador de frames
    framec += 1

    # Pular frames de acordo com a condição
    if framec % frame_p != 0:
        continue  # Pula a execução do restante do loop até atingir o frame certo

    # Acessar as detecções
    detections = results[0].boxes
    pessoa_errada_detectada = False  # Variável para saber se pessoa errada foi detectada

    for detection in detections:
        class_id = int(detection.cls)  # ID da classe
        confidence = detection.conf  # Confiança da detecção
        
        # Imprimir o ID da classe e a confiança
        print(f"Class ID: {class_id}, Confidence: {confidence}")

        if class_id == 1:  # Ajuste o ID da classe conforme necessário
            pessoa_errada_detectada = True
            print("Pessoa errada detectada")
            try:
                # Enviar comando para acender o LED no ESP32
                print("enviando...")
                requests.get(esp32_url, params={"comando": "pessoa_errada"})
                print("Pronto....")
            except Exception as e:
                print(f"Erro ao enviar comando para o ESP32: {e}")
    
    # Caso não tenha detectado 'pessoa errada', enviar comando para desligar o LED
    if not pessoa_errada_detectada:
        try:
            # Enviar comando para apagar o LED
            print("enviando comando de apagar LED")
            requests.get(esp32_url, params={"comando": "apagar_led"})
            print("PRONTO")
        except Exception as e:
            print(f"Erro ao enviar comando para o ESP32: {e}")
    time.sleep(2)    
    print("fim for")
    
# Liberar a câmera e fechar as janelas
camera.release()
cv2.destroyAllWindows()
