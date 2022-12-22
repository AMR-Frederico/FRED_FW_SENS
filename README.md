# 🎩 *Frederico - código sensores*

## 🏎️ Descrição da categoria
O Frederico é um robô da categoria **_Trekking_** (ou **_Robomagellan_**, em inglês), que deve ser capaz de se localizar e locomover em um espaço determinado. 


### **Principais regras**
**Características do robô:**

- Dimensões máximas: 500x500x500 (cm)
- Peso: sem restrição
- Controle: Autônomo

**Características do campo:**

- Dimensões campo: 18x20 (m)
- A competição será realizada em gramado aberto, portanto estará sujeito as condições do campo e do clima.
- O robô deve ser capaz de identificar e alcançar 3 marcos definidos por uma chapa de metal branca de dimensões 1000x1000x2 (mm).
- Poderá, ou não, ser solicitado um cone para facilitar a identificação dos objetivos.

**Características de prova:**

- Duração: 10 min
- O robô deve alcançar os objetivos e ordem crescente (início para marco 1, marco 1 para marco 2 e marco 2 para marco 3);
- ***Ao redor do objetivo 3 terá obstáculos***, localizados a uma distância máxima de 5m do marco 3, sem tamanho pré definido e que deverão ser contornados, e não transpostos.
- Ao atingor cada marco, ***com pelo uma parte do robô tocando o marco***, o robô deverá sinalizar por meio de um dispositivo luminoso, vísivel e aparente, antes de prosseguir para o próximo objetivo. Ao chegar e sinalizar a chegada no último objetivo, o tempo será parado.
- Ganha o robô que terminar o circuito mais rápido, ou o que avance mais no trajeto.

** **
## O código: 
Para a versão atual do projeto, temos um ESP32 dedicado à leitura e processamento de sensores, sendo eles:

* __Detecção de objetos:__ sensor ultrassom (HC-SR04) e sensor a laser (VL53L0X)

* __Orientação:__ sensor inercial (MPU-6050)

Além disso, temos a implentação de alguns filtros dimiuir o ruído na leitura: 
* __Filtro de mediana__ 
* __Filtro média móvel__

Após concluída a leitura dos sensores, este ESP32 se comunica com o ROS, responsável por toda a odometria, mapeamento e navegação do robô (abaixo estará o link para estes repositórios). 

## Pins 
Abaixo estão os pinos de conexão do ESP32
| Pinos ESP32 1 | Conexão |
|:-------------:|:-------:|
|  `VIN`        | +5V     |
|  `GND`        | GND     |
|  `D36`        | ECHO 1  |
|  `D34`        | ECHO 2  |
|  `D32`        | ECHO 3  |
|  `D25`        | TRIG 1  |
|  `D27`        | TRIG 2  |
|  `D12`        | TRIG 3  |
|  `D22`        | SCL     |
|  `D21`        | SDA     |
|  `D19`        | XSHUNT 1|
|  `D18`        | XSHUNT 2|
|  `D17`        | RX      |
|  `D16`        | TX      |

** **

## Tópicos publicados
Os tópicos publicados por este ESP estão listados abaixo: 

**Sensor ultrassom:** Distância dos sensores em metros
`sensor/ultrasonic/left/distance`

`sensor/ultrasonic/middle/distance`

`sensor/ultrasonic/right/distance`

**Sensor inerical:** Orientação do robô, por meio dos angulos de Euler, no eixo Yaw

`sensor/imu/yaw`

## 💻 Reposítórios relacionados
Demais repositórios relacionados os projeto estão listados abaixo

**ESP32 Scripts**

[🔗 ESP32 - Potência](https://github.com/DinossauroBebado/FRED_FW_POT.git)

**ROS Scripts (Raspberry)**

[🔗 Hardware interface](https://github.com/DinossauroBebado/fred_hardware_interface.git)

[🔗 Move_base - safe twist](https://github.com/DinossauroBebado/fred_move_base.git)


** **

## 🌐 Links úteis

[🔗 Regras Robocore](https://robocore-eventos.s3.sa-east-1.amazonaws.com/public/Regras+-+Trekking+Indoor.pdf)

### Convenções ROS

[🔗 REP 103 - Standard Units of Measure and Coordinate Conventions](https://www.ros.org/reps/rep-0103.html)

[🔗 REP 105 - Coordinate Frames for Mobile Platforms](https://www.ros.org/reps/rep-0105.html)

[🔗 REP 145 - Conventions for IMU Sensor Drivers](https://www.ros.org/reps/rep-0145.html#data-reporting)