import pygame, math, numpy, socket, os, threading, time

ROBOT_RADIUS = 20
DRAW_OFFSET = 20
START_RANGE_X = [0,100]
STOP_RANGE_X = [500,600]
RANGE_Y = [560,600]
PORT = 8084
STOP_BTN_PRESSED = 0
START_BTN_PRESSED = 1
NO_BTN_PRESSED = -1
ADC_VOLTAGE_CHUNK = 3.3/1023.0
BTN_THREAD_SLEEP = 0.001

lidar_data = []
connection = 0
address = 0

def draw_start_and_stop(screen, font):
    text_start = font.render('START', True, pygame.Color(0,0,0))
    text_stop = font.render('STOP', True, pygame.Color(0,0,0))
    pygame.draw.rect(screen, pygame.Color(0,255,0),[0,560,100,40])
    pygame.draw.rect(screen, pygame.Color(255,0,0),[500,560,100,40])
    screen.blit(text_start, (15,570)) 
    screen.blit(text_stop, (520,570))

def da_to_pos(distance, angle, robot_pos):
    obstacle_x = distance * numpy.cos(angle) + robot_pos[0]
    obstacle_y = -distance * numpy.sin(angle) + robot_pos[1]
    return(int(obstacle_x), int(obstacle_y))

def conv_voltage_to_distance(voltage):
    distance = 0.0
    if(voltage > 1.9):
        distance = 10.0
    elif(voltage <= 1.9 and voltage > 1.02):
        distance = -11.36 * voltage + 31.59
    elif(voltage <= 1.02 and voltage > 0.8):
        distance = -45.45 * voltage + 66.36
    elif(voltage <= 0.8 and voltage > 0.55):
        distance = -80.0 * voltage + 94.0
    elif (voltage <= 0.55 and voltage > 0.35):
        distance = -500.0 * voltage + 325.0
    else:
        distance = -1.0
    return distance

def get_angle_from_duty_cycle(duty_cycle):
    angle_deg = (duty_cycle - 2.5) * 18.0
    angle_rad = angle_deg * math.pi / 180.0
    return angle_rad

def convert_data(data):
    conv_data = []
    for i in range(len(data)//2):
        voltage = data[2*i] * ADC_VOLTAGE_CHUNK
        distance = conv_voltage_to_distance(voltage)
        angle = get_angle_from_duty_cycle(data[2*i+1])
        conv_data.append(distance)
        conv_data.append(angle)
    return conv_data

def screen_update():
    prev_distance = -1.0
    prev_point = (0,0)
    screen.fill((255, 255, 255))
    for i in range(len(lidar_data)//2):
        if (lidar_data[2*i] > 0):
            point = da_to_pos(lidar_data[2*i] + ROBOT_RADIUS + DRAW_OFFSET, lidar_data[2*i+1], (300,300))
            if prev_distance > 0: 
                pygame.draw.line(screen, pygame.Color(0,0,0), prev_point, point, width=2)
            prev_point = point
            prev_distance = lidar_data[2*i]
        else:
            prev_point = (0,0)
            prev_distance = -1.0
    pygame.draw.circle(screen, pygame.Color(0, 0, 200), (300, 300), ROBOT_RADIUS)
    pygame.draw.circle(screen, pygame.Color(0,0,0), (300,300), ROBOT_RADIUS, width=2)
    draw_start_and_stop(screen, font)
    pygame.display.flip()

def check_start_stop():
    press_flag = NO_BTN_PRESSED
    mouse_pos = pygame.mouse.get_pos()
    for ev in pygame.event.get(): 
        if ev.type == pygame.MOUSEBUTTONDOWN: 
            if (START_RANGE_X[0] <= mouse_pos[0] <= START_RANGE_X[1]) and (RANGE_Y[0] <= mouse_pos[1] <= RANGE_Y[1]):
                press_flag = START_BTN_PRESSED    
            if (STOP_RANGE_X[0] <= mouse_pos[0] <= STOP_RANGE_X[1]) and (RANGE_Y[0] <= mouse_pos[1] <= RANGE_Y[1]):
                press_flag = STOP_BTN_PRESSED
    return press_flag

def button_check_thread(name):
    global connection
    global lidar_data
    while True:
        buffer = connection.recv(1024)  
        print("Received data !")
        buf_decoded = buffer.decode('utf-8')
        buf_decoded.replace(" ", "")
        data = numpy.asarray(buf_decoded.split(","))
        data = data.astype(float)
        lidar_data = convert_data(data)
        screen_update()

                
# Inicijalizacija ekrana na kome se prikazuje mapirani prostor            
pygame.init()
screen = pygame.display.set_mode((600,600))
sysfont = pygame.font.get_default_font()
font = pygame.font.SysFont(sysfont, 32)
draw_start_and_stop(screen, font)
pygame.display.flip()
print("Screen initialized !")

# Kreiranje web socket-a za razmjenu poruka sa vozilom
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.bind(('0.0.0.0', PORT))  
sock.listen(1)
print("Server started !")

# Kreiranje niti koja obradjuje pritisak na START i STOP tastere
thrd = threading.Thread(target=button_check_thread, args=(1,))

connection,address = sock.accept()
thrd.start()

while True: 
    btn_check = check_start_stop()
    if btn_check == START_BTN_PRESSED:
        print("START button pressed !")
        connection.send(bytes("%", "utf-8"))
    elif btn_check == STOP_BTN_PRESSED:
        connection.send(bytes("#", "utf-8"))
        print("STOP button pressed !")

