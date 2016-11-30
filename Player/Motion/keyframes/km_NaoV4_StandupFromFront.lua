local mot = {};
mot.servos = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22};
mot.keyframes = {
  {
    angles = {0.000, 0, 1.571, 0.262, -1.571, 0.000, 0.000, 0.000, -0.349, 0.000, 0.000, 0.000, 0.000, 0.000, -0.349, 0.000, 0.000, 0.000, 1.571, -0.262, 1.571, 0.000, },
    duration = 0.600;
  },
  {
    angles = {0.000, 0.000, 1.571, 1.571, -1.571, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1.571, -1.571, 1.571, 0.000, },
    duration = 0.600;
  },
  {
    angles = {0.000, 0.000, -1.571, 1.571, -1.571, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, -1.571, -1.571, 1.571, 0.000, },
    duration = 0.600;
  },
  {
    angles = {0.000, 0.000, -1.571, 0.000, -1.571, 0.000, 0.000, 0.000, 0.000, 0.000, -0.436, 0.000, 0.000, 0.000, 0.000, 0.000, -0.436, 0.000, -1.571, 0.000, 1.571, 0.000, },
    duration = 0.600;
  },
  {
    angles = {-0.576, 0.000, -1.571, 0.000, -1.571, 0.000, 0.000, 0.000, 0.000, 2.094, -1.134, 0.000, 0.000, 0.000, 0.000, 2.094, -1.134, 0.000, -1.571, 0.000, 1.571, 0.000, },
    duration = 0.350;
  },
  {
    angles = {-0.576, 0.000, -1.571, 0.000, -1.571, 0.000, 0.000, 0.000, 0.000, 2.094, -0.960, -0.052, 0.000, 0.000, 0.000, 2.094, -0.960, 0.052, -1.571, 0.000, 1.571, 0.000, },
    duration = 0.350;
  },
  {
    angles = {-0.349, 0.000, -0.175, 0.000, -1.571, -1.134, -0.873, 0.000, -1.571, 1.047, -0.785, -0.122, -0.873, 0.000, -1.571, 1.047, -0.785, 0.122, -0.175, 0.000, 1.571, 1.134, },
    duration = 0.600;
  },
  {
    angles = {-0.338, 0.184, -0.152, 0.293, -1.327, -1.549, -1.135, -0.354, -1.543, 2.146, -1.218, -0.095, 0.000, -0.034, -1.144, 0.914, -0.337, 0.328, 0.287, 0.001, 1.781, 1.551, },
    duration = 0.700;
  },
  {
    angles = {-0.336, 0.482, 0.762, 0.301, -1.417, -1.559, -1.051, -0.373, -1.390, 2.108, -1.221, -0.009, 0.000, 0.356, -1.407, 1.031, 0.166, 0.453, 0.491, -0.014, 1.462, 0.528, },
    duration = 0.700;
  },
  {
    angles = {-0.233, -0.776, 0.400, 0.009, -1.444, 0.034, -1.144, -0.362, -1.456, 2.149, -0.799, 0.140, 0.000, 0.213, -1.129, 0.781, 0.512, 0.293, 0.989, -0.930, 1.440, 1.556, },
    duration = 0.700;
  },
  {
    angles = {-0.176, -0.546, 1.072, 1.075, -1.256, -0.324, -1.066, -0.293, -0.911, 2.152, -0.715, 0.322, 0.000, -0.265, -1.111, 1.069, 0.512, 0.153, 1.002, -0.891, 1.419, 1.534, },
    duration = 0.700;
  },
  {
    angles={
-0.107421875,-0.59523391723633,0.94950413703918,0.82831811904907,-0.99867582321167,-1.1811380386353,-1.1458560228348,-0.37732195854187,-0.81757998466492,2.1153440475464,-0.62437987327576,0.27616190910339,0,0.05679988861084,-1.2870678901672,2.1246318817139,-0.36658406257629,-0.23619413375854,0.97413206100464,-0.85448002815247,0.96791195869446,1.5325078964233,
    },
duration = 0.6; 
  },
  {
    angles={
-0.019984006881714,-0.30684185028076,1.4526560306549,0.82678413391113,-0.99714183807373,-1.1780700683594,-0.23926210403442,-0.1978440284729,-1.4618600606918,2.1168780326843,-0.65659403800964,0.11662602424622,0,0.11969399452209,-1.6138100624084,2.1230978965759,-0.53379011154175,-0.17023205757141,0.97413206100464,-0.85601401329041,0.68105411529541,1.5325078964233,
    },
duration = 0.55; 
  },
  {
    angles={
0.016832113265991,-0.33292007446289,1.16936,0,1.43117,-1.535889,-0.18864011764526,-0.073590040206909,-0.67491817474365,1.1811380386353,-0.48632001876831,0.024585962295532,0,-0.15795993804932,-0.618243932724,1.2103681564331,-0.59208202362061,0.11816000938416,1.16936,0,-1.43117,1.535889,
    },
duration = 0.6; 
  },
};

return mot;