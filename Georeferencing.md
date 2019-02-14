
**What is georefrencing?**

Georeferencing is the process of transforming coordinate reference system(CRS) of raster data into the new real-world coordinates system. Coordinate system of the raster dataset that is to be georeferenced is known as source coordinate system(CRS) and CRS after georeferenced or output of georeferencing is known as destination CRS. Coordinates can be found in two different ways, one is that if there is mark in the map itself i.e. coordinates can be found in the map and the next is to have a field survey and point out the coordinated by the help of some identifiable things in the image or in the map. After having the ground control point(GCP), by the help of transformation functions raster data in source CRS is transformed to destination CRS. For the better transformation GCPs should be taken on the four corners of the image and some identifiable points in the middle of the image which may not be always possible but can result good transformation. For georeferencing, Geospatial Data Abstraction Library(GDAL) plugin is used which is a core QGIS plugin i.e. it will be already installed just need an activation.

**How to do georeferencing?**

Raster image to be georeferenced: -
