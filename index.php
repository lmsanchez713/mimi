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
  <!-- your content here... -->
  <div id="saida">TESTANDO</div>
  <!-- <script src="js/scripts.js"></script> -->
  <script>
    var ws = new WebSocket('wss://ws.binaryws.com/websockets/v3?app_id=1089');

    // var elemento_de_saida = document.getElementById("saida").innerHTML+= "new content"

    ws.onopen = function(evt) { ws.send(JSON.stringify({ticks:'R_100'})); };

    ws.onmessage = function(msg) { var data = JSON.parse(msg.data); console.log('ticks update: %o', data);};
  </script>
</body>
</html>