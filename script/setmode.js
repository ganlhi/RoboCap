const SerialPort = require('serialport')

const port = new SerialPort('COM3', {
  parser: SerialPort.parsers.readline('\n')
})

module.exports = function SetMode(mode) {
  port.on('data', data => {
    data = data.trim()
    console.log(`Data: "${data}"`)

    if (data === 'INIT OK') {
      port.write(`${mode}`, () => console.log(`Mode set: ${mode}`))
    }

    if (data === `${mode}`) {
      port.close()
    }
  })

  port.on('error', err => console.log('Error: ', err.message))
}

