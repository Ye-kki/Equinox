let Red, Green, Blue;

function kelvinToRGB(Temperature) {
Temperature = Temperature / 100

  if(Temperature <= 66) Red = 255;
  else {
    Red = Temperature - 60;
    Red = 329.698727446 * (Red ** -0.1332047592);
    if (Red < 0) Red = 0;
    if (Red > 255) Red = 255;
  }

  if (Temperature <= 66){
    Green = Temperature;
    Green = 99.4708025861 * Math.log(Green) - 161.1195681661;
    if (Green < 0) Green = 0;
    if (Green > 255) Green = 255;
  }
  else {
    Green = Temperature - 60;
    Green = 288.1221695283 * (Green ** -0.0755148492);
    if (Green < 0) Green = 0;
    if (Green > 255) Green = 255;
  }

  if (Temperature >= 66) Blue = 255;
  else {
    if (Temperature <= 19) Blue = 0;
    else {
      Blue = Temperature - 10;
      Blue = 138.5177312231 * Math.log(Blue) - 305.0447927307;
      if (Blue < 0) Blue = 0;
      if (Blue > 255)  Blue = 255;
    }
  }
}

var kelvin = new iro.ColorPicker(".kelvin", {
    width: 315,
    sliderSize: 20,
    borderWidth : 3,
    borderColor : "#FFFFFF",
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
    borderWidth : 3,
    borderColor : "#787878",
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

const background = document.querySelector(".background");
const blurCircle = document.querySelector(".blur-circle");
const lightCircle = document.querySelector(".light-circle");

kelvin.on(["color:init", "color:change"], function (color) {
    // console.log('K' + parseInt(color.kelvin));
    kelvinToRGB(color.kelvin);
    console.log(Red);
    console.log(Green);
    console.log(Blue);

    gsap.to(background, 0.7, {
      background: "linear-gradient(179.99deg, rgba(0, 0, 0, 0.9) -9.21%, rgba(" + Red + ', ' + Green + ', ' + Blue + ", 0.3) 37%, rgba(0, 0, 0, 0.9) 76.28%)",
    });
    gsap.to(blurCircle, 0.7, {
      border: "40px solid rgba(" + Red + ', ' + Green + ', ' + Blue + ", 0.5)",
    });
    gsap.to(lightCircle, 0.7, {
      border: "5px solid rgb(" + (Red+70) + ', ' + (Green+70) + ', ' + (Blue+70) + ")",
    });
});

value.on(["color:init", "color:change"], function (color1) {
    console.log('B' + parseInt(color1.value/100*255));
});