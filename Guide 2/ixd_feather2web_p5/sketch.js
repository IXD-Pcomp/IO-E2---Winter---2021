let url = 'yourAPIfeedINFOgoesHere';
let counter = 0;

function setup() {
    createCanvas(255, 255);
}

function draw() {
    fill(255, 10);
    rect(0, 0, width, height);
    if (counter % 80 == 0) {
        getData();
    }
    counter++;
}

function getData() {
    let data;
    httpGet(url, 'json', function (response) {
        console.log(response);
        data = response.last_value;
        let resize = map(data, 1, 0, 25, 150);
        noStroke();
        fill(255, 0, 0);
        ellipse(width / 2, height / 2, resize);
        console.log(data);
    });
}
