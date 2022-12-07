import cv2
import imutils
from PIL import Image
import numpy as np

#size of the window and the background image in pixels
WINDOW_WIDTH = 600
WINDOW_HEIGHT = 600

#size of the robot image in pixels
ROBOT_WIDTH = 40
ROBOT_HEIGHT = 60

#position offset so the robot image is aligned with the field
CENTER_X = 300
CENTER_Y = 500

#how big the field is in the robot's units
FIELD_WIDTH = 5090
FIELD_HEIGHT = 5090

#delay in milliseconds between screen updates
DELAY = 50

data = []
#load data from robot
with open("replay.txt") as f:
    data = f.read().split(',')

def tryToConvertToInt(value):
    try:
        return int(float(value))
    except:
        return 0

data = [tryToConvertToInt(data[i]) for i in range(len(data))]
data.pop(-1)

#load images for GUI
background = np.zeros((2,2))
with Image.open("Vex Field.png") as im:
    background = np.array(im)
    background = cv2.resize(background, (WINDOW_WIDTH, WINDOW_HEIGHT), cv2.INTER_AREA)
    background = cv2.cvtColor(background, cv2.COLOR_RGB2BGR)

robot_img = np.zeros((2,2))
with Image.open("Robot.png") as im:
    robot_img = np.array(im)
    robot_img = cv2.resize(robot_img, (ROBOT_WIDTH, ROBOT_HEIGHT), cv2.INTER_AREA)

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
        print("Done")

    background_copy = background.copy()
    robot_img_copy = robot_img.copy()

    robot_img_copy = imutils.rotate_bound(robot_img_copy, rob_rotation)

    for x in range(robot_img_copy.shape[0]):
        for y in range(robot_img_copy.shape[1]):
            if robot_img_copy[x, y, 3] != 0:
                try:
                    background_copy[x+rob_y+CENTER_Y, y-rob_x+CENTER_X] = robot_img_copy[x][y][0:3]
                except IndexError:
                    pass

    cv2.imshow('Replay', background_copy)
    key = cv2.waitKey(DELAY)

    if key == 27:
        break

    counter+=3


cv2.destroyAllWindows()