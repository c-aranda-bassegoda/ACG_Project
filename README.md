# ACG_Project

Project built on a framework provided for one of the assignments for the RUG Advanced Computer Graphics course.
To be run with Qt Creator.
This projects implements a variation on Catmull-Clark subdivision that allows for variable sharpness of edges.
Supports both integer and non-integer sharpness.
Interface includes options to change the amount of subdivision steps and the sharpness of the entire mesh.
There is no interface for editing the sharpness of individual edges, but the subdivision logic is implemented.
For testing purposes the function to update edge sharpness is MainWindow::on_SharpSteps_valueChanged, and this can be modified to hardcode individual edges.
