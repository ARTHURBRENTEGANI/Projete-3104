import cv2

print("teste")

camera = cv2.VideoCapture(0,cv2.CAP_DSHOW)
print ("foi")

if not camera.isOpened():
    print("Erro ao abrir a câmera.")

while True:
    status, frame = camera.read()
    print ("camera")
    if not status:
        print("Não foi possível capturar o frame.")
        break
    cv2.imshow("Camera", frame)
    if cv2.waitKey(1)==ord('q'):
        break


camera.release()
cv2.destroyAllWindows()
