import numpy as np
import cv2




if __name__ == '__main__':
    positions2d = np.load('vr2d.npy')
    positions3d = np.load('vr3d.npy')

    objpoints = [np.array(positions3d, dtype=np.float32)]
    imgpoints = [np.array(positions2d, dtype=np.float32)]
    
    img = cv2.imread("img2.png")
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Assert initial focal length guess
    focal_length_initial = 100
    camera_matrix = np.array([ [focal_length_initial, 0.0, 960.0] ,[0.0, focal_length_initial, 540.0], [0.0, 0.0, 1.0]])
    # Create the x y z and origin axis to project in 2D
    retval,camera_matrix, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints,img_gray.shape,camera_matrix, None, None, None, cv2.CALIB_USE_INTRINSIC_GUESS + cv2.CALIB_FIX_PRINCIPAL_POINT)
    axis = np.float32([[1, 0, 0],[0, 1, 0],[0, 0, -1],[0, 0, 0]]).reshape(-1,3)
    # Retrive translation and rotation matrices
    print(camera_matrix)
    ret, rvecs, tvecs = cv2.solvePnP(positions3d, positions2d, camera_matrix, None)
    imgpts, jacobian = cv2.projectPoints(axis, rvecs, tvecs, camera_matrix, None)
    # Draw the projected axis
    img = cv2.line(img, (int(imgpts[3][0][0]), int(imgpts[3][0][1])), (int(imgpts[0][0][0]), int(imgpts[0][0][1])), (255,0,0), 5)
    img = cv2.line(img, (int(imgpts[3][0][0]), int(imgpts[3][0][1])), (int(imgpts[1][0][0]), int(imgpts[1][0][1])), (0,255,0), 5)
    img = cv2.line(img, (int(imgpts[3][0][0]), int(imgpts[3][0][1])), (int(imgpts[2][0][0]), int(imgpts[2][0][1])), (0,0,255), 5)

    cv2.imwrite("estimatedImg2.jpg", img)