# pressureGaugeDetection
Usage: First Line in main holds the image filename which contains a pressure guage as a part of the image. The path of the file is relative to build folder so make sure the path to image is correct.

Working: Uses a circle detector by ommiting concentric circles followed by a needle detector. The needle detector looks for two lines about the circles center whose length is close to the radius of the circle. The algorithm ensures the lines are close to the circle center. The intersection of the two line gives us the reading point which is then connected to the center to show the final green line for reading.
