package model;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;

import java.io.PrintWriter;
import java.util.Arrays;

public class SerialArduinoConnection {
	private final SerialPort arduinoPort;

	public SerialArduinoConnection() {
		for (SerialPort port : SerialPort.getCommPorts()) {
			System.out.println(port.toString());
		}
		arduinoPort = (SerialPort) Arrays.stream(SerialPort.getCommPorts())
		                                 .filter(serialPort -> "IOUSBHostDevice (Dial-In)".equals(serialPort.toString()))
		                                 .toArray()[0];
		boolean result = arduinoPort.openPort();
		System.out.println(result ? "port opened!" : "port NOT opened!");
	}

	public void sendString(String data) {
		System.out.printf("Sending %s to Arduino...\n", data);
		PrintWriter writer = new PrintWriter(arduinoPort.getOutputStream());
		writer.print(data);
		writer.close();
	}

	public byte[] receiveBytes() {
		byte[] newData = new byte[arduinoPort.bytesAvailable()];
		arduinoPort.readBytes(newData, newData.length);
		System.out.print("Receiving data from Arduino:");
		for (byte newDatum : newData) {
			System.out.printf("%c", (char) newDatum);
		}
		System.out.println();
		return newData;
	}

	public void addDataListener(SerialPortDataListener dataListener) {
		arduinoPort.addDataListener(dataListener);
	}
}
