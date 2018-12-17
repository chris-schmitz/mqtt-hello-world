var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

io.on('connection', function(socket){
  console.log('a user connected');
  io.emit('test', {test: 'worked'})
});

http.listen(3001, function(){
  console.log('listening on *:3000');
});