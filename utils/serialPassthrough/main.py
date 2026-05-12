import serial
import pygame
import sys

# 1. Initialize Serial Connection to Arduino
# Replace '/dev/ttyACM0' with your actual Arduino port
try:
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.1)
except Exception as e:
    print(f"Serial Error: {e}")
    sys.exit(1)

# 2. Initialize Pygame Window
pygame.init()
WIDTH, HEIGHT = 400, 300
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Pi Passthrough - Click to Grab Input")

# Track grab state
input_grabbed = False
clock = pygame.time.Clock()

print("Running... Click inside the window to capture mouse/keyboard.")
print("Press ESCAPE to release input control.")

while True:
    # Maintain 60Hz polling rate for smooth tracking
    clock.tick(60) 
    
    # Handle Events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            ser.close()
            pygame.quit()
            sys.exit()

        # Click inside window to engage lock
        elif event.type == pygame.MOUSEBUTTONDOWN and not input_grabbed:
            pygame.event.set_grab(True)
            pygame.mouse.set_visible(False)
            input_grabbed = True
            print("Input GRABBED.")

        # Keyboard Input Processing
        elif event.type in (pygame.KEYDOWN, pygame.KEYUP):
            # Check for escape key to break input lock
            if event.key == pygame.K_ESCAPE and event.type == pygame.KEYDOWN:
                pygame.event.set_grab(False)
                pygame.mouse.set_visible(True)
                input_grabbed = False
                print("Input RELEASED.")
                continue

            if input_grabbed:
                # Format: K,[KeyID],[1 for Down / 0 for Up]\n
                # Example: K,97,1 (A key pressed)
                is_down = 1 if event.type == pygame.KEYDOWN else 0
                payload = f"K,{event.unicode},{is_down}"
                ser.write(payload.encode('ascii'))
                print(payload.encode('ascii'))

    lb = 0
    rb = 0
    # Mouse Input Processing (Only active during active lock)
    if input_grabbed:
        # get_rel() fetches relative movement since the last loop tick
        dx, dy = pygame.mouse.get_rel()
        
        # Fetch current button array states: (Left, Middle, Right)
        buttons = pygame.mouse.get_pressed()
        rb = 1 if buttons[2] else 0 # Pygame index 2 is Right-click
        lb = 1 if buttons[0] else 0
        
        # Only broadcast serial data if an actual movement or click state changed
        if dx != 0 or dy != 0 or lb != old_lb or rb != old_rb:
            # Format matches your Arduino translation engine layout
            payload = f"M,{lb},{rb},{dx},{dy}\n"
            ser.write(payload.encode('ascii'))
            print(payload.encode('ascii'))

        old_lb = lb
        old_rb = rb

    # Visual UI Feedback Layout
    screen.fill((30, 30, 30))
    if input_grabbed:
        # Green background indicates active capture session
        screen.fill((20, 80, 20)) 
    pygame.display.flip()
