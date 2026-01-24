import tkinter as tk
from PIL import Image, ImageTk
import time
import os

# Obtener ruta absoluta del directorio del script
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
INTERFAZ_PATH = os.path.join(SCRIPT_DIR, 'interfaz.txt')

etiquetas_all = []
ventana = tk.Tk()
ventana.title('GAME')

def actualizar_contenido():
	with open(INTERFAZ_PATH, 'r') as archivo:
		data = archivo.readlines()
		version = data.pop(0)
		size = data.pop(0).strip().split(',')
		filas = int(size[0])
		columnas = int(size[1])

		tile_size = (20, 20)
		screen = ventana.winfo_screenwidth()
		if screen >= 1920:
			tile_size = (40, 40)
		elif screen >= 1280:
			tile_size = (30, 30)

		# Leer la matriz de nombres de imágenes
		matriz_imagenes = []
		for raw in data:
			linea = raw.strip()
			imagenes = linea.split(',')
			matriz_imagenes.append(imagenes)
		id_imagen = 0
		for i in range(filas):
			for j in range(columnas):
				imagen = Image.open(matriz_imagenes[i][j])
				imagen = imagen.resize(tile_size, Image.LANCZOS)
				imagen_tk = ImageTk.PhotoImage(imagen)


				etiquetas_all[id_imagen].config(image=imagen_tk)
				etiquetas_all[id_imagen].image = imagen_tk
				id_imagen+=1
	
	ventana.after(200, actualizar_contenido)





def cargar_interfaz():
	tile_size = (20, 20)
	screen = ventana.winfo_screenwidth()
	if screen >= 1920:
		tile_size = (40, 40)
	elif screen >= 1280:
		tile_size = (30, 30)
	try:
		with open(INTERFAZ_PATH, 'r') as archivo:
			# Leer el número de interfaz
			numero_interfaz = int(archivo.readline().strip())

			# Leer las dimensiones de la matriz
			dimensiones = archivo.readline().strip().split(',')
			filas = int(dimensiones[0])
			columnas = int(dimensiones[1])

			# Crear la ventana de la interfaz
			# ventana = tk.Tk()
			# ventana.title(f'Interfaz {numero_interfaz}')

			# Leer la matriz de nombres de imágenes
			matriz_imagenes = []
			for i in range(filas):
				linea = archivo.readline().strip()
				imagenes = linea.split(',')
				matriz_imagenes.append(imagenes)

			# Crear y mostrar la matriz de imágenes
			imagenes_tk = []  # Lista para mantener referencias a las imágenes
			for i in range(filas):
				fila_imagenes_tk = []  # Lista para almacenar las imágenes de la fila actual
				for j in range(columnas):
					imagen = Image.open(matriz_imagenes[i][j])
					imagen = imagen.resize(tile_size, Image.LANCZOS)
					imagen_tk = ImageTk.PhotoImage(imagen)
					fila_imagenes_tk.append(imagen_tk)
					etiqueta = tk.Label(ventana, image=imagen_tk)
					etiqueta.grid(row=i, column=j)
					etiquetas_all.append(etiqueta)	

				imagenes_tk.append(fila_imagenes_tk)

		ventana.after(200, actualizar_contenido)
		ventana.mainloop()

	except FileNotFoundError as e:
		print(f"Error: {e}")
	except Exception as e:
		print(f"Error inesperado: {e}")

cargar_interfaz()

