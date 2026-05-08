import serial
import pygame
import sys

try:
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=0.1)
except Exception as e:
    print(f"Error: {e}")
    sys.exit(1)

pygame.init()

WIDTH, HEIGHT = 400, 300
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Pi Passthrough - Click to grab input")

input_grabbed = False
