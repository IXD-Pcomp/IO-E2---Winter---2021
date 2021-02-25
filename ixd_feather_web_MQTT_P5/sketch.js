/////////////////////////////////////////////////
////////////////Written by Steve Hudak///////////
/////////////////////////////////////////////////

//// you need two seperate Adafruit IO data feeds with different
//// names, you need to post data and get data seperately. Buttons or
//// interactions in the p5 sketch are collected in the outData 
//// variable and sent to the server using the sendData function. 
//// Buttons from the Feather are sent to Adafruit and the getData 
//// function keeps checking it for new data.


//// This line decleares the variable url1 and defines it to get data
//// from an Adafuit.io feed you set up, you need to replace 'yourName' 
//// and 'yourFeed' with your specific account name and feed name 
var url1 = 'https://io.adafruit.com/api/v2/yourName/feeds/yourFeed';
//var url1 = 'https://io.adafruit.com/api/v2/IO_DW/feeds/feed1';


//// This line decleares the variable url2 and defines it to post data 
//// to an Adafuit.io feed you set up, you need to replace 'yourName' with 
//// your specific account name and 'yourFeed' with your specific feed name 
var url2 = 'https://io.adafruit.com/api/v2/yourName/feeds/yourFeed/data';
//var url2 = 'https://io.adafruit.com/api/v2/IO_DW/feeds/feed2/data';

//// this starts a boolean true/false and checks if the mouse clicks 
//// is true, place it anywhere you want a mouseclick by adding 
//// 'clicked == true'  to an if statement and then reset it by adding 
//// 'clicked == false' to the else statement 
var clicked = false;

function mouseClicked() {
    clicked = true;
};


///// declares the counter variable and defines it to start at zero
var counter = 0;
///// declares the outData variable and defines it to start at zero
var outData = '0';
//// declares and defines the variables I want to use to change the
//// size of the ellipses, these will change based on mapping the 
//// inData value to change the size of the ellipses
var bubble1 = 20;
var bubble2 = 20;

function setup() {
    //// make the canvas whatever size you require
    createCanvas(800, 240);
}

function draw() {

    //// sets the colour of the background and redraws it each frame    
    background("#28d1d1");

    //// sets the colour of an ellipse and redraws it each frame
    fill("#e0ab1b");
    //// sets the location on the canvas and size of an ellipse 
    //// and redraws it each frame
    ellipse(width / 3, height / 2, bubble1);

    //// sets the colour of another ellipse and redraws it each frame
    fill("#eb40eb");
    //// sets the location on the canvas and size of an ellipse and
    //// redraws it each frame
    ellipse(width - width / 3, height / 2, bubble2);

    //// declaring and defining two variables that check for
    //// distance using location and mouse position as conditions 
    var d1 = dist(width / 3, height / 2, mouseX, mouseY);
    var d2 = dist(width - width / 3, height / 2, mouseX, mouseY);

    //// uses the above variable and a mouseclicked event to
    //// identify if someone has clicked inside the location 
    //// of the ellipses to:
    if (d1 < 100 && clicked == true) {
        //// change the colour of the ellipse and increases the 
        //// effect of the bubble value
        fill("#eb4040");
        ellipse(width / 3, height / 2, bubble1 += 10);
        //// then it checks the last state of the button was '0' or '2'
        //// and changes it to a '1' to identify the buttons as different 
        //// from each other, this number will be sent to Adafruit IO in 
        //// the outData variable each time it reads a 'clicked == true' - so
        //// each mouseclick at the specific ellipse locations
        if (outData === '0' || outData === '2') {
            outData = '1';
            console.log('1');
        } else {
            fill("#e0ab1b");
            outData = '0';
            console.log('0');
            clicked = false;
        }
        //// this calls/runs the sendData function and assigns the new data to it  
        sendData(outData);
    }


    //// duplicates the above logic to identify if someone has clicked inside the 
    //// location of the other ellipse:
    if (d2 < 100 && clicked == true) {
        //// change the colour of the ellipse and increases the 
        //// effect of the bubble value
        fill("#eb4061");
        ellipse(width - width / 3, height / 2, bubble2 += 10);
        //// then it checks the last state of the button was '0' or '1'
        //// and changes it to a '2' to identify the buttons as different 
        //// from each other, this number will be sent to Adafruit IO in 
        //// the outData variable each time it reads a 'clicked == true' - so
        //// each mouseclick at the specific ellipse locations
        if (outData === '0' || outData === '1') {
            outData = '2';
            console.log('2');
        } else {
            fill("#eb40eb");
            outData = '0';
            console.log('0');
            clicked = false;
        }
        sendData(outData);
    }


    /////counts to 1000 before continuing, 
    ///////this slows down how quickly it accesses 
    ///////Adafruit IO and avoides throttling
    if (counter % 500 == 0) {
        getData();
    }
    //// starts the counter increment
    counter++;
}

//// this function gets the Feather data from the Adarfuit IO feed
function getData() {

    let inData;
    httpGet(url1, 'json', function (response) {
        console.log(response)
        inData = response.last_value;

        //// this if-statement checks the current value of the data and if
        //// its a 1 tells bubble1 to map it from a 0 = 20 to 1 = 200
        if (inData == 1) {
            bubble1 = map(inData, 1, 0, 200, 20);
            //bubble2 = 20;
            //// this else-if-statement checks the current value of the data and if
            //// its a 2 tells bubble2 to map it from a 0 = 20 to 2 = 200        
        } else if (inData == 2) {
            bubble2 = map(inData, 2, 0, 200, 20);
            //bubble1 = 20;
            //// this else-statement checks the current value of the data and if
            //// its a not a either of the if values tells bubble1 and bubble2 to 
            //// stay at 20      
        } else {
            bubble1 = 20;
            bubble2 = 20;
        }
    });

}

//// this function sends the current p5 outData value so each mouse click event sends either 0, 1 or 2 
function sendData(outData) {
    let postData = {
        "value": outData,
        ///// you ONLY need to replace 'yourKey' with your aio Adafruit IO key here:
        "X-AIO-Key": "your adafruit key here"
    };
    httpPost(url2, 'json', postData, function (result) {
        console.log(result);
    });
}
