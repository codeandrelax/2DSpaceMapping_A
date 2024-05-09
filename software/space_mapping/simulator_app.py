import asyncio
from websockets.server import serve
from websockets.sync.client import connect
import pygame, math, numpy

ROBOT_RADIUS = 20
START_RANGE_X = [0,100]
STOP_RANGE_X = [700,800]
RANGE_Y = [760,800]
ROBOT_IP_ADDR = "192.168.100.75"
PORT = 8765

lidar_data = []

pygame.init()
screen = pygame.display.set_mode((800,800))
sysfont = pygame.font.get_default_font()
font = pygame.font.SysFont(sysfont, 32)
pygame.display.flip()

# Function that converts LIDAR data given in distance and angle 
# to the Cartesian coordinates of the obstacle in the space.
def da_to_pos(distance, angle, robot_pos):
    obstacle_x = distance * numpy.cos(angle) + robot_pos[0]
    obstacle_y = -distance * numpy.sin(angle) + robot_pos[1]
    return(int(obstacle_x), int(obstacle_y))


async def receiver(websocket):
    global lidar_data
    async for message in websocket:
        data_temp = numpy.asarray(message.split(","))
        if len(data_temp) > 1:
            lidar_data = data_temp.astype(float)
        #print("--------------------------------------------------------------------------------------")
        #print(lidar_data)
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
    pygame.display.flip()

async def main():
    async with serve(receiver, "0.0.0.0", PORT):
        await asyncio.Future()

print("Server ON")
asyncio.run(main())