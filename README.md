# QEqualizerView
Simple equalizer widget for Qt

## Usage

This is a equalizer widget based on QCustomPlot for Qt.

_**Use [LiquidEqualizerWidget](https://github.com/ThePBone/LiquidEqualizerWidget) instead!**_

Since the `QEqualizerView` class inherits QCustomPlot, you can use its functions to read/write the eq graph.
(Refer to the [docs](https://www.qcustomplot.com/documentation/) if you don't know how)

### Customization
The widget is a 15-band equalizer by default. However, you can customize the amount and the interval of these bands.
To do that, go to the file `qequalizerview.h` and modify the following variables/statements:

|          Variable         	|                         Description                         	|
|-------------------------	  |-----------------------------------------------------------	  |
| `NUM_BANDS`               	| amount of bands                                             	|
| `double bands[]`          	| numeric values (used as x-coordinates)                      	|
| `QStringList bands_str[]` 	| string representation of these values (displayed as labels) 	|

### Public signals
Because this class implements its own editing logic, it contains with two signals which can be used to check if a slider has been dragged.

Of course QCustomPlot provides a lot more signals, check their docs for more information.

| Signal              	| Description                                        	|
|---------------------	|----------------------------------------------------	|
| `EditingFinished()` 	| mouse has been released                            	|
| `DataChanged()`     	| mousebutton is held down and data is being changed 	|

### Access values
You can use the interface of QCustomPlot to access the values of the graph like this:
      
      ui->eqplot->graph(0)->data()->...

Now you could iterate though the keys (hz/x-values) to find a value or update an eq-band manually.
Check the QCustomPlot [docs](https://www.qcustomplot.com/documentation/classQCPDataContainer.html) for more information...
## Screenshot
![](https://raw.githubusercontent.com/ThePBone/QEqualizerView/master/screenshot.png)
