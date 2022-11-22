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
          sliderType: 'value'
        }
      },
    ]
  });
  
kelvin.on(["color:init", "color:change"], function (color) {
    console.log('K' + parseInt(color.kelvin));
});

value.on(["color:init", "color:change"], function (color1) {
    console.log('B' + parseInt(color1.value/100*255));
});