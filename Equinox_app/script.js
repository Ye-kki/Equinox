var kelvin = new iro.ColorPicker(".kelvin", {
    width: 315,
    sliderSize: 20,
    borderWidth : 2,
    borderColor : "#3A3A3A",
    handleRadius : 15,
    layout: [
      { 
        component: iro.ui.Slider,
        options: {
          // can also be 'saturation', 'value', 'red', 'green', 'blue', 'alpha' or 'kelvin'
          sliderType: 'kelvin'
        }
      },
    ]
  });

var value = new iro.ColorPicker(".value", {
    width: 315,
    sliderSize: 20,
    borderWidth : 2,
    borderColor : "#3A3A3A",
    handleRadius : 15,
    layout: [
      { 
        component: iro.ui.Slider,
        options: {
          // can also be 'saturation', 'value', 'red', 'green', 'blue', 'alpha' or 'kelvin'
          sliderType: 'value'
        }
      },
    ]
  });
  

  kelvin.on(["color:init", "color:change"], function (color) {
  });

  value.on(["color:init", "color:change"], function (color) {
});