import time

def leer(name):
    with open(name) as a:

        lista = a.readlines()
    return lista
while True:
    
    data = leer("interfaz.txt")
    for i in range(len(data)):
        #print(data[i])
        data[i] = data[i].replace("media/protoman.png","xxx")
        data[i] = data[i].replace("media/rockman.png","media/protoman.png")
        data[i] = data[i].replace("xxx","media/rockman.png")
    #print(data)
    data = "".join(data)
    with open("interfaz.txt","w") as a:
        a.write(data)
    time.sleep(0.2)

