import pygame
import sys

# Inicializamos
pygame.init()
pantalla = pygame.display.set_mode((600, 600))
pygame.display.set_caption("Prueba de Ratón")
reloj = pygame.time.Clock()

# Ocultamos el cursor normal de Windows/Linux para ver solo nuestro círculo
pygame.mouse.set_visible(False)

while True:
    # 1. Eventos
    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # 2. Input del ratón
    # Obtenemos las coordenadas X e Y exactas del cursor
    mouse_x, mouse_y = pygame.mouse.get_pos()

    # Obtenemos qué botones se están presionando: (Izquierdo, Rueda, Derecho)
    botones = pygame.mouse.get_pressed()

    # Decidimos el color del círculo según el botón presionado
    if botones[0]:  # Clic Izquierdo
        color = (255, 0, 0)  # Rojo
    elif botones[2]:  # Clic Derecho
        color = (255, 255, 0)  # Amarillo
    elif botones[1]:  # Clic Rueda central
        color = (0, 255, 0)  # Verde
    else:  # Ningún clic
        color = (0, 150, 255)  # Azul celeste (por defecto)

    # 3. Dibujar
    pantalla.fill((30, 30, 30))  # Fondo oscuro

    # Dibujamos el círculo: (Pantalla, Color, (Coordenada X, Coordenada Y), Radio)
    pygame.draw.circle(pantalla, color, (mouse_x, mouse_y), 25)

    # 4. Actualizar pantalla
    pygame.display.flip()
    reloj.tick(60)
