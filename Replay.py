import cv2
import imutils
from PIL import Image
import numpy as np
from time import sleep

WINDOW_WIDTH = 600
WINDOW_HEIGHT = 600

DELAY = 100 #delay in milliseconds between screen updates

data = []
#load data from robot
with open("replay.txt") as f:
    data = f.read().split(',')

data = [int(data[i]) for i in range(len(data))]

#load images for GUI
background = np.zeros((2,2))
with Image.open("Vex Field.png") as im:
    background = np.array(im)
    background = cv2.resize(background, (WINDOW_WIDTH, WINDOW_HEIGHT), cv2.INTER_AREA)
    background = cv2.cvtColor(background, cv2.COLOR_RGB2BGR)

robot_img = np.zeros((2,2))
with Image.open("Triangle.png") as im:
    robot_img = np.array(im)
    robot_img = cv2.resize(robot_img, (70, 70), cv2.INTER_AREA)

#position offset so the robot image is aligned with the field
CENTER_X = 10
CENTER_Y = 10

#how big the field is in the robot's units
FIELD_WIDTH = 1000
FIELD_HEIGHT = 1000

rob_x = 0
rob_y = 0
rob_rotation = 0

counter = 0

while True:
    if counter < len(data):
        #scaling the locations to fit on the field
        rob_x = int((data[counter] / FIELD_WIDTH) * WINDOW_WIDTH)
        rob_y = int((data[counter+1] / FIELD_HEIGHT) * WINDOW_HEIGHT)
        rob_rotation = int(data[counter+2])
    else:
        counter-=3 #keep the counter from increasing any further

    background_copy = background.copy()
    robot_img_copy = robot_img.copy()

    robot_img_copy = imutils.rotate(robot_img_copy, 180 + rob_rotation)

    for x in range(robot_img_copy.shape[0]):
        for y in range(robot_img_copy.shape[1]):
            if robot_img_copy[x, y, 3] != 0:
                background_copy[x+rob_y+CENTER_Y, y+rob_x+CENTER_X] = robot_img_copy[x][y][0:3]

    cv2.imshow('Replay', background_copy)
    key = cv2.waitKey(DELAY)

    if key == 27:
        break

    counter+=3


cv2.destroyAllWindows()