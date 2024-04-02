import env, sensor
import pygame
import math
import time
import asyncio
from websockets.sync.client import connect

IP_ADDRESS = "10.99.146.210"
PORT = ":8765"

enviroment = env.buildEnviroment((512,1024))
enviroment.originalMap = enviroment.map.copy()
lidar = sensor.LaserSensor(300, enviroment.originalMap)
enviroment.infomap = enviroment.map.copy()

running = True

time.sleep(1)

def send_lidar_data(message):
    with connect("ws://" + IP_ADDRESS + PORT) as websocket:
        websocket.send(message)
        #print("------------------------------------------------------------------------------------")
        #print(message)


while running:
    message = ""
    sensor_on = False
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if not pygame.mouse.get_focused():
            sensor_on = False
        else:
            sensor_on = True

        if sensor_on:
            position = pygame.mouse.get_pos()
            lidar.position = position
            sensor_data = lidar.sense_obstacles()
            enviroment.store_data(sensor_data)
            enviroment.show_data()
            for i in range(len(sensor_data)):
                #angle_deg = int(180 * sensor_data[i][1] / math.pi)
                #print("Distance =", sensor_data[i][0], "Angle =", angle_deg, "°")
                distance_str = str(sensor_data[i][0])[0:5]
                angle_str = str(sensor_data[i][1])[0:7]
                message += distance_str + "," + angle_str + ","
            send_lidar_data(message[:-1])
    enviroment.map.blit(enviroment.infomap, enviroment.externalMap.get_rect(center = enviroment.map.get_rect().center))
    pygame.display.update()


    time.sleep(1/lidar.rps)
