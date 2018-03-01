'use strict';

let mysql = require('mysql');
let connection = mysql.createConnection({
  host : '127.0.0.1',
  user : 'teamb',
  password : 'password',
  port : 3306,
  database: 'TrainDB'
});

connection.connect();

connection.query('SELECT * from TrainLog ;', (err, rows, fields) => {
  if (err) throw err;

  console.log('The solution is: ', rows);
});

connection.end();