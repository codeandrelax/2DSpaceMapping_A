import numpy as np
import math
import pygame

class LaserSensor:

    def __init__(self, Range, map):
        self.Range = Range; 
        self.rps = 10; # obrtaji po sekundi
        self.position = (0,0)
        self.map = map
        self.W, self.H = pygame.display.get_surface().get_size()
    
    def distance(self, obstaclePosition):
        px = (self.position[0] - obstaclePosition[0])**2
        py = (self.position[1] - obstaclePosition[1])**2
        distance = math.sqrt(px + py)
        return distance

    def sense_obstacles(self):
        data = []
        x1, y1 = self.position[0], self.position[1]
        for angle in np.linspace(0, 2*math.pi, 160, False):
            x2, y2 = (x1 + self.Range * math.cos(angle), y1 - self.Range * math.sin(angle))
            for i in range(0, 100):
                u = i / 100
                x = int(x2 * u  + x1 * (1 - u))        
                y = int(y2 * u  + y1 * (1 - u)) 
                if 0 < x < self.W and 0 < y < self.H:
                    color = self.map.get_at((x,y))
                    if (color[0], color[1], color[2]) == (0,0,0):
                        distance = self.distance((x,y))
                        output = [distance, angle]
                        output.append(self.position)
                        data.append(output)
                        break
        return data
        
