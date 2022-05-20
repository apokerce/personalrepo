import numpy as np
import cv2






def find_best_focal_length(fx_1, fy_1, points3d, points2d):
    old_fx = old_fy = old_errorx = old_errory = 0
    num_step = 1000
    learning_rate = 0.001
    error_x = 0
    error_y = 0
    for i in range(num_step):
        camera_matrix1 = np.array([ [fx_1, 0.0, 960.0] ,[0.0, fy_1, 540.0], [0.0, 0.0, 1.0]])
        _ , rvecs1, tvecs1 = cv2.solvePnP(points3d, points2d, camera_matrix1, None)
        
        for i in range(20):
            imgpts2, _ = cv2.projectPoints(points3d[i], rvecs1, tvecs1, camera_matrix1, None)
            error = np.squeeze(points2d[i] - imgpts2)
            error_x += error[0]
            error_y += error[1]
        # Calculate the change of rate of focal lengths and errors
        derivativex = fx_1 - old_fx
        derivativey = fy_1 - old_fy
        old_fx = fx_1
        old_fy = fy_1
        derivative_errorx = error_x - old_errorx
        derivative_errory = error_y - old_errory
        old_errorx = error_x
        old_errory = error_y
        # Apply change of rate to find new fx and fy
        fx_1 = fx_1 + learning_rate*(derivative_errorx/derivativex)
        fy_1 = fy_1 + learning_rate*(derivative_errory/derivativey)
    
    return fx_1, fy_1


if __name__ == '__main__':
    positions2d = np.load('vr2d.npy')
    positions3d = np.load('vr3d.npy')
    img = cv2.imread("img3.png")
    # Assert initial focal length guess
    focal_length_initial = 100

    # Reproject the points until we found a good estimation of fx and fy
    fx, fy = find_best_focal_length(focal_length_initial, focal_length_initial, positions3d, positions2d)
    camera_matrix = np.array([ [fx, 0.0, 960.0] ,[0.0, fy, 540.0], [0.0, 0.0, 1.0]])
    # Create the x y z and origin axis to project in 2D
    axis = np.float32([[1, 0, 0],[0, 1, 0],[0, 0, -1],[0, 0, 0]]).reshape(-1,3)
    # Retrive translation and rotation matrices
    ret, rvecs, tvecs = cv2.solvePnP(positions3d, positions2d, camera_matrix, None)
    imgpts, jacobian = cv2.projectPoints(axis, rvecs, tvecs, camera_matrix, None)
    # Draw the projected axis
    img = cv2.line(img, (int(imgpts[3][0][0]), int(imgpts[3][0][1])), (int(imgpts[0][0][0]), int(imgpts[0][0][1])), (255,0,0), 5)
    img = cv2.line(img, (int(imgpts[3][0][0]), int(imgpts[3][0][1])), (int(imgpts[1][0][0]), int(imgpts[1][0][1])), (0,255,0), 5)
    img = cv2.line(img, (int(imgpts[3][0][0]), int(imgpts[3][0][1])), (int(imgpts[2][0][0]), int(imgpts[2][0][1])), (0,0,255), 5)

    cv2.imwrite("estimatedImg3.jpg", img)