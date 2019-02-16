What is Geoserver ? 

Geoserver is the open server platform which is written in Java that allows users to create, share, edit the geospatial data which contains both raster and vector data. The main benefit of using then geoserver is it is free, can handle large amount of datasets and produces a high quality maps. Geoserver also contains very high quality documentation for its use which can be found on (https://docs.geoserver.org/). In our project, we have mapped the bin co-ordinates of the Moers City into the geoserver. 


Installing a Geoserver in Linux computers 

1. At first we need to install the Java Runtime Environment(JRE) in order to install the Geoserver. Geoserver requires the Java 8 environment and Java 9 is still not supported by the geoserver. Either Java Runtine Environment(JRE) or Java Development Kit( JDK) will be good to go. Both Oracle JDK and Open JDK will be okay.

Installing  JDK 8 

- Open your terminal and type these commands :: 

sudo add-apt-repository ppa:webupd8team/java
sudo apt-get update
sudo apt-get install oracle-java8-installer

- If there are more than one version of java in your system then, type this command at first. 

sudo update-alternatives --config java

Note : You nay need to specify the java version at first and then ID of the JDK 8 which has just been installed. 

The first step is completed, installing Java Development Kit is the basic platform for the geoserver to run. 

In order to check wether the Java has been installed in your computer of nott. Open your terminal and type. This command line will check the JDK version. 

java -version 

It will give results like: 





2. After installing the JDK, its time to install the geoserver. Go to the Geoserver download page on the official site of the geoserver. If you are not sure about which version to download, go for the PLATFORM INDEPENDENT BINARY. (Platform Independent Binary is a web application of the geoserver which a light weight and portable application server and it works easily on all the system that is why is it mostly popularly used)

Go to the http://geoserver.org/release/stable/ . It will open the download page of the geoserver. Click on the Platform Independent Binary and zip file will be automatically downloaded. 


It will open this page and you can see the Packages topic in the top part, below that you can see the platform independent binary link, click that link and download will start automatically. You can also download the zip file or tar.gz from the Source Code but  platform independent binary is easily to install and works on every system. 




















3. After the zip file has been downloaded, you can see “geoserver-2.14.2-bin.zip” in your download folder. Then open the terminal again and type:

unzip geoserver-2.14.2-bin.zip

It will unzip all the files inside the zip folder and creat a seperated folder in the preferred location but here I have kept in the Downloads folder. After these all steps have been completed there will be other subfolders inside the geoserver-2.14.2 folder which are bin , etc, lib and others. 

4. After that add an environment varaible in the location where you have the geoserver folder unzipped in the earlier stage. Type these commands in your terminal::

echo		"export GEOSERVER_HOME=/home/shirish/Downloads/geoserver-2.14.2" >> ~/.profile

You have to provide the location where you have kept the geoserver folder, here I have kept in the downloads so I have given the Downloads path to the Geoserver_home.  After that type this command:

. ~/.profile

It will guide us to the environment variable profile which we have just created. 

5. Make yourself the owner of the geoserver folder which we have just created. For example : 

sudo chown -R USER_NAME /usr/share/geoserver/

Type this command in the termnal and change the USER_NAME to your name and the location to your geoserver location. In my case I have done something like this: 

sudo chown -R Shirish /home/shirish/Downloads/geoserver-2.14.2/

6. After that almost every process is completed its time to run the geoserver. Direct your terminal to the geoserver folder location and type these commands::

cd geoserver/bin
sh startup.sh 

7. Open the web browser and type http://localhost:8080/geoserver

You can see a web page loaded which looks something like this: 







 If you see this webpage, then geoserver is finally wotking in your system. Not its time work on the geoserver, 


Using the Geoserver web administration interface 

Geoserver consists of the web interface which consits of the layers, stores, layers preview and other options for the create and edit of the maps. 

http://localhost:8080/geoserver

It is the default installation of the server. It will direct you to a default geoserver page. When the web application starts, there will be a display page which looks like this: 



1. For the LOGGING IN.  Default administration credentiaons are:

Username : admin
Password : geoserver 

Note : There can be changed in the security departmnet which can be found in https://docs.geoserver.org/latest/en/user/security/index.html#security

2. After giving the username and password there will be another webpage. There will be list of options in the left side. 





3. Layer Preview : There is option called layer preview in the left side of the interface. Layer Preview gives the option to view the output of the published layers. There will be the list of the default layers in the Layer Preview option.




As you can from the figure above there are Title named “World Rectangle”, “Manhattan (NY) Roads”, “North America Sample Imagery” and others. If I click on the OpenLayers option which is in the Common Formats column. It will open a layer on the another tab quickly showing the layer it contains. For example, If I click on the North America Sample Imagery then it will show the map like this: 






Similarly if I click on the “Manhattan(NY) landmarks” another map will open. So Layer Preview will help us to display the maps whicha are altready there in the geoserver databases. 


Pushing a shape file 

After getting to know about the basic interface of the geoserver, its time to add some shape files which we have created in our QGIS project. (QGIS stands for Quantum Geographic Information System. QGIS is a cross platform, open sourrce software which is designed for the viewing, editing, and adding the geospatial data. QGIS allows you to add layers, maps, data and many other maps related things.) 




Basic overview of the previos work: 

- We have got the csv file which contains the longtitude and latitude of the dustbins of the city moers. We can add that csv file into the QGIS by Add Delimited text layer from the layer option. 

- After adding the csv file we need to convert it into the shape file for that we have to install the plugin call “Point Sampling Tool” (which may not be in the QGIS plugin sections so we have to download it seperately. This plugin can be found here: https://plugins.qgis.org/plugins/pointsamplingtool/ . After that in Manage and Install plugins sections in the Plugin option. We can install zip file directly from our coumputer. 

Here you can see the option Install from ZIP. Click that and upload the zip file we have just downloaded and tick mark that plugin. 



- Using that plugin we can convert our csv file into the shape file. As a result we have got the shape file. This shape file we will use in the geoserver. This shape file contains the longitude and the latitude of the dustbins in the city moers.

Moving on to Geoserver again as we have got the shape file. 







1. Creating a new workspace

The first step is to creat the workspace for the shapefile. Workspace can be defined as the containers which groups all the similar layers together in to one.  

- Go to the Data>Workspace. 

- Click on the Add new workspace button.

- There will be another box add the workspace name and the Namespace URI.




















-  In my case I have written,
	
	Name : moers_bin
	Namespace URI : http://geoserver.org/moers_bin

Tick mark  the Default Workspace as your current working workspace. 

- At the end select the submit button and the newly created moers_bin will be added to the workspace list.


As you can see workspace “moers_bin” is added to the workspace list.  


2.  Creating a Store

After we are done with the workspace creation, we have to add a new store. The store guides the shape file to add new layers and other options. 

- Go to Data>Stores

- There you can see the list of stores , workspaces and their type they belong to.

- In order to create a new shape file, click on the Add new store option and it will show another page.




- Click on the Shapefile.

- Another Vector Data store will open where you need to fill some information about the shape files.

*  Select the workspace to moers_bin

*  Enter the Data Source Name as http://geoserver.org/moers_bin

* Enter a short description.

*  In the Connection Parameters select the shape file we have created earliers.
Note : You can double check the shape file by right clicking and looking at the attributes table there you can find the longitude and latitude coordinates.

*  After completeing everything save the shape file.


After everthing is done the shape file looks like this. After the creation of shape file click on save and it will take you to another page. 






3. Creating a Layer


As soon as we save our shape file this new layer box will appear. Now our store is loaed with the values, we can publish the layer. For that we need to create a new layer. 


-  Click on publish on the action column.

-  The Edit layer defines the data parameters to the layers. You can enter a short Title and an abstract.

I have given the layer name and Title  as Newnewnewshapefiles and also provided the short Abstract. 

-  Now for Generating the layers bounding boxes. Click the Compute from data and then Compute from native bounds links.


- Click on the publishing tab located at the top of the table.

- Select Point on the Default Style. 






























- Finally after completing everything click on Save located at the bottom of the page.

Our shape file has been created, edited and ready to be view. We have just loaded the shape file containing the latitude and longitude in the Geoserver. 


4.  Viewing a layer

- Go to the Layer Preview option in the left side and you can find moers_bin:Newnewnewshapefile in the list.




- Select the OpenLayers link in the Common Formats column.

- Another tab will open which will show all the longitude and latitude values. There are zoom in and zoom out options in order to look more closely. The figure is displayed below. We have just selected the shape file and made changes in the geoserver.










Adding a Web Map Service (WMS) 


Web map service (WMS) is a service provided by the geoserver which helps to add the set of map layers. We can add number of styles, layers and projections by the help of wms. 

For creating a new layer we can use wms. Steps for adding layers are: 
- Go to the Data>Store.  Click on the WMS located at the bottom of the page.

- Select moers_bin as the Workspace and in the WMS Source Name write http://geoserver.org/moers_bin and add some short description.



























- In the Capabilities URL write https://ows.mundialis.de/services/service?

Note: You can even find this link here: https://www.mundialis.de/en/ows-mundialis/









Once we have added the layer the job here now is to merge the two parts. The shape file and the wms layer inorder to give us the results what are are looking for. 



Grouping Layers

Layers grouping can be defined as merging the two layers into one. Here we have one layer which contains the shape file of longitude and latitude and another layer that contains the map. 

- Go to Data>Layers Groups and click on the Add new layer group.





















- Add the Name, Title and Abstract of the layer group.

- Choose moers_bin on the workspace.

- Click on Generate Bounds  to generate the bounds for the layers.





- After that click on Add Layers to add the layers. We have got two layers here. Here moers_bin:TOPO-OSM-WMS1 is the Web Map Service layer and moers_bin:Newnewnewshapefiles in our shape file layer. WMS layer is kept at the top because it needs to get plotted at first and then only shape file points can be plotted. 



- After completing everything click on save and go the Layer Preview.



Prevewing Results

After grouping the results we have in total of three layers and these layers can be seen in the Layers Preview. 



From the icon we can say that the shape file containts the point as its Default Style, WMS layer contains the map and the moers_bin_location1 contains two layers in it. 

- Click the OpenLayer in the common formats.

Another tab will open which contains the result as whole. Both the layers and you can see the points in the red with the map below it. 




























What is Georeferencing in QGIS ? 


Georeferencing is process of providing real world coordinates and values to each and every pixel in raster data. Georeferencing is mainly used in collecting the longitude and latitude coordinates of the raster data. We have to give the fixed location and by the help of  georeferencing we can get the fixed latitude and longitude value of the desired location. It is mainly used in field map survery where we need GPS value of certain location. While doing georeferencing a special care should be taken while selecting points in the map so that we can achieve high accuracy in georeferencing and produce a high accurate map with accurate coordinates. 

Overview of the project:: In this project we are doing the georeferencing in certain image. We have map of city moers in which there is pictures of the bin located in the image. We have to find the exact longitude and latitude value of bins in the image. In the image we can see the yellow dots representing the bins in the image. Our task is to find exact longitude and latitude of those bins which are present in the map. For that purpose we need georeferencing. 





We will use the map of moers for this project. This map has been provided to us by Moers city center department’s people. 

Steps for georeferencing

1. First thing first for using the georeferencing we need to install the “Georeferencer GDAL” plugin. This is a core plugin, which is already installed in your QGIS you just need to enable this plugin in order to use it. To enable this plugin go to Plugins>Manage and Install plugins. You will see the list of the plugins in ALL section then search for georeferencer GDAL plugin. Tick mark that plugin and this will enable the plugin.




2. The georeferencer plugin is installed in the Raster Menu. Click on the Raster>Georeferencer to open the plugin.












3. After clicking the georeferencer plugin, anohter window will add up. This the georeferencer window. This window consist of two sections: top section where raster will be displayed and bottom section where  Ground Control Points (GCP) will be shown.



4.  Now to open our image file. Go to the Files>Open Raster. Browse and select the image that we need to find the location of.













5.  After that you will see map being loaded in the raster section of georeferencer.  In my case I have uploaed map of moers and that map is being shown in the raster section. 





6. Now its time to do some setting changes. In the georeferencer click on Transformation Settings. You will see another box.

Make sure you choose these settings before beginning your process: 

Transformation type : Thin Plate Spline
Resampling method : Nearest neighbour
Target SRS : EPSG 4647

In the Output raster, browse and selec where you want to show your output raster file. 

































7. You can either Zoom in or zoom out and control the map with options that are being provided in the georeferencer tool bar.











8.  Now our task is to assign the coordinates of the bin in the map. For this click on Edit>Add Point in the toolbar and click on one of the bin location in the map.  You will see the cursor sign in being changed. Lets Suppose I click on the orange point which is near to number 3. 






9. Another window will pop up asking about the Enter Map Coordinates. You can see From map canvas option below. Click on that option.






10.  After clicking From map canvas option, you will be redirected to the QGIS main page whete you need to first intall the OpenLayer plugin.  For that go to plugins>Manage and install plugin there you can at first install the OpenLayer plugin. After installing the plugin you can find that installed plugin in Web>OpenLayers plugin.

To open the map, go to the Web>OpenLayers plugin>OpenStreetMap. After this you can see a world map being loaded in the raster of QGIS. Zoom into the Germany and Moers city as we are dealing with the map of moers in this project. You can use Zoom in option and find Moers which is located at the West Germany. 

After that you can match the image file we have with the map of moers in the QGIS. And we have already selected a point in the georeferencer and try to find the same exact location in the map so that we could get the accurate point and accurate latitutde and longitude values. 







11. After clicking on the accurate location we will get the coordinates of that point and that points will be reflected in the georeferencer. The Enter Map Coordinates will show the coordinates of the points that we have just clicked. Click on OK. 



















12.  You can find that GCP table now will have a row with details about location.






13.  Similarly, add more locations from the map with the similar process. More the number of points, more the accurate image and results. In the figure below you can see number of points being taken and it will be added in the list. 




14.  Go to the File>Start Georeferencing to get our things started.
















15. Once georeferencing has been completed you can see georeferenced layer in the QGIS.



















16.  Finally after taking number of points we finally complete out georeferecing part. Always zoon in to take the point and choose the points in the building as they are easy to locate in the actual map and take many points to get the accurate result. Always make sure that the EPSG remains same for all. In this I have take EPSG 4647. 
























