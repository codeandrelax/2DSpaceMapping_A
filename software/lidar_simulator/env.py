import math
import pygame
import numpy as np

class buildEnviroment:
    def __init__(self, MapDimensions):
        pygame.init()
        self.pointCloud = []
        self.externalMap = pygame.image.load('map.png')
        self.map_H, self.map_W = MapDimensions
        self.map = pygame.display.set_mode((self.map_W, self.map_H))
        self.map.blit(self.externalMap, self.externalMap.get_rect(center = self.map.get_rect().center))

    # Function that converts LIDAR data given in distance and angle from current
    # position to the Cartesian coordinates of the obstacle in the space.
    def da_to_pos(self, distance, angle, robotPosition):
        obstacle_x = distance * np.cos(angle) + robotPosition[0]
        obstacle_y = -distance * np.sin(angle) + robotPosition[1]
        return(int(obstacle_x), int(obstacle_y))

    # For the testing purpose.
    def store_data(self, data):
        for element in data:
            point = self.da_to_pos(element[0], element[1], element[2])
            if point not in self.pointCloud:
                self.pointCloud.append(point)
    
    # For the testing purpose.
    def show_data(self):
        self.infomap = self.map.copy()
        for point in self.pointCloud:
            self.infomap.set_at((int(point[0]),int(point[1])), (255,0,0))