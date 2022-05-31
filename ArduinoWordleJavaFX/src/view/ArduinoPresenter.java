package view;

import model.SerialArduinoConnection;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;

public class ArduinoPresenter implements SerialPortDataListener {
	private final SerialArduinoConnection model;
	private final ArduinoView view;

	public ArduinoPresenter(SerialArduinoConnection model, ArduinoView view) {
		this.model = model;
		this.view = view;
		addEventListeners();
		model.addDataListener(this);
	}

	private void addEventListeners() {
		view.getBtnSend().setOnAction(actionEvent -> {
			System.out.println(Thread.currentThread());
			view.getTfTextFromArduino().setText("");
			model.sendString(view.getTfTextForArduino().getText());
		});
	}

	@Override
	public int getListeningEvents() {
		return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
	}

	@Override
	public void serialEvent(SerialPortEvent event) {
		System.out.println(Thread.currentThread());
		if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
			System.out.println(event.getEventType());
			return;
		}
		StringBuilder text = new StringBuilder(view.getTfTextFromArduino().getText());
		for (byte oneByte : model.receiveBytes()) {
			text.append((char) oneByte);
		}
		Platform.runLater(() -> view.getTfTextFromArduino().setText(text.toString()));
	}
}
