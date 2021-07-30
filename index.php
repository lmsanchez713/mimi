<!doctype html>

<html lang="pt-br">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>A Basic HTML5 Template</title>
  <meta name="description" content="A simple HTML5 Template for new projects.">
  <meta name="author" content="SitePoint">

  <meta property="og:title" content="A Basic HTML5 Template">
  <meta property="og:type" content="website">
  <meta property="og:url" content="https://www.sitepoint.com/a-basic-html5-template/">
  <meta property="og:description" content="A simple HTML5 Template for new projects.">
  <!-- <meta property="og:image" content="image.png"> -->

  <!-- <link rel="icon" href="/favicon.ico"> -->
  <!-- <link rel="icon" href="/favicon.svg" type="image/svg+xml"> -->
  <!-- <link rel="apple-touch-icon" href="/apple-touch-icon.png"> -->

  <!-- <link rel="stylesheet" href="css/styles.css?v=1.0"> -->

</head>

<body>
  
<canvas id="myChart" width="400" height="400"></canvas>
<script>
var ctx = document.getElementById('myChart').getContext('2d');
var myChart = new Chart(ctx, {
    type: 'bar',
    data: {
        labels: ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'],
        datasets: [{
            label: '# of Votes',
            data: [12, 19, 3, 5, 2, 3],
            backgroundColor: [
                'rgba(255, 99, 132, 0.2)',
                'rgba(54, 162, 235, 0.2)',
                'rgba(255, 206, 86, 0.2)',
                'rgba(75, 192, 192, 0.2)',
                'rgba(153, 102, 255, 0.2)',
                'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
                'rgba(255, 99, 132, 1)',
                'rgba(54, 162, 235, 1)',
                'rgba(255, 206, 86, 1)',
                'rgba(75, 192, 192, 1)',
                'rgba(153, 102, 255, 1)',
                'rgba(255, 159, 64, 1)'
            ],
            borderWidth: 1
        }]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});
</script>

  <div id="saida">TESTANDO</div>
  <!-- <script src="js/scripts.js"></script> -->
  <script>
    var ws = new WebSocket('wss://ws.binaryws.com/websockets/v3?app_id=1089');

    // var elemento_de_saida = document.getElementById("saida").innerHTML+= "new content"

    ws.onopen = function(evt) { ws.send(JSON.stringify({ticks:'R_100'})); };

    ws.onmessage = function(msg) { var data = JSON.parse(msg.data); console.log('ticks update: %o', data);};
  </script>
    <script src="http://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.5.0/chart.js"></script>
</body>
</html>