import asyncio
from websockets.server import serve
from websockets.sync.client import connect
import pygame, math, numpy

ROBOT_RADIUS = 20
START_RANGE_X = [0,100]
STOP_RANGE_X = [700,800]
RANGE_Y = [760,800]
ROBOT_IP_ADDR = "192.168.100.75"

# ASSERT IF SIMULATOR IS USED
SIMULATOR = 0

lidar_data = []

def draw_start_and_stop(screen, font):
    text_start = font.render('START', True, pygame.Color(0,0,0))
    text_stop = font.render('STOP', True, pygame.Color(0,0,0))
    pygame.draw.rect(screen, pygame.Color(0,255,0),[0,760,100,40])
    pygame.draw.rect(screen, pygame.Color(255,0,0),[700,760,100,40])
    screen.blit(text_start, (15,770)) 
    screen.blit(text_stop, (720,770))

pygame.init()
screen = pygame.display.set_mode((800,800))
sysfont = pygame.font.get_default_font()
font = pygame.font.SysFont(sysfont, 32)
draw_start_and_stop(screen, font)
pygame.display.flip()

# Function that converts LIDAR data given in distance and angle 
# to the Cartesian coordinates of the obstacle in the space.
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
        distance = conv_voltage_to_distance(data[2*i])
        angle = get_angle_from_duty_cycle(data[2*i+1])
        if distance > 0.0:
            conv_data.append(distance)
            conv_data.append(angle)
    return conv_data


async def receiver(websocket):
    global lidar_data
    async for message in websocket:
        data_temp = numpy.asarray(message.split(","))
        if SIMULATOR == 0:
            data_temp = convert_data(data_temp)
        if len(data_temp) > 1:
            lidar_data = data_temp.astype(float)
        #print("--------------------------------------------------------------------------------------")
        #print(lidar_data)
        await check_start_stop()
        await screen_update()

async def screen_update():
    screen.fill((255, 255, 255))
    for i in range(len(lidar_data)//2):
        point = da_to_pos(lidar_data[2*i] + ROBOT_RADIUS, lidar_data[2*i+1], (400,400))
        #print("Distance:", lidar_data[i] + ROBOT_RADIUS)
        #print("Angle:", lidar_data[i+1])
        #print("Point:", point)
        #print("----------------------------------------------------------------------------------------")
        pygame.draw.circle(screen, pygame.Color(0,0,0), point, 4)
    pygame.draw.circle(screen, pygame.Color(0, 0, 200), (400, 400), ROBOT_RADIUS)
    pygame.draw.circle(screen, pygame.Color(0,0,0), (400,400), ROBOT_RADIUS, width=2)
    draw_start_and_stop(screen, font)
    pygame.display.flip()

async def check_start_stop():
    mouse_pos = pygame.mouse.get_pos()
    for ev in pygame.event.get(): 
        if ev.type == pygame.MOUSEBUTTONDOWN: 
            #print("Mouse position:", mouse_pos)
            if (START_RANGE_X[0] <= mouse_pos[0] <= START_RANGE_X[1]) and (RANGE_Y[0] <= mouse_pos[1] <= RANGE_Y[1]):
                print("USER PRESSED THE START BUTTON")
                #with connect("ws://" + ROBOT_IP_ADDR + "8765") as websocket:
                #    websocket.send("1")
            if (STOP_RANGE_X[0] <= mouse_pos[0] <= STOP_RANGE_X[1]) and (RANGE_Y[0] <= mouse_pos[1] <= RANGE_Y[1]):
                print("USER PRESSED THE STOP BUTTON")
                #with connect("ws://" + ROBOT_IP_ADDR + "8765") as websocket:
                #    websocket.send("0")

async def main():
    async with serve(receiver, "0.0.0.0", 8765):
        await asyncio.Future()

print("Server ON")
asyncio.run(main())