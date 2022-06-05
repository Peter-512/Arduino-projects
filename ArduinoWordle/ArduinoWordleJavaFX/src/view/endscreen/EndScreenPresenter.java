package view.endscreen;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;
import javafx.stage.Stage;
import model.SerialArduinoConnection;

public class EndScreenPresenter implements SerialPortDataListener {
	private SerialArduinoConnection model;
	private EndScreenView view;

	public EndScreenPresenter(SerialArduinoConnection model, EndScreenView view) {
		this.model = model;
		this.view = view;

		model.addDataListener(this);
		addEventHandlers();
		updateView();
	}

	private void addEventHandlers() {
		view.getCloseGame().setOnAction(actionEvent -> {
			Stage stage = (Stage) view.getScene().getWindow();
			stage.close();
		});
	}

	private void updateView() {
		view.getGuesses()
		    .setText(String.format("%d guess%s", model.getGuesses(), model.getGuesses() == 1 ? " only!" : "es"));
	}

	@Override
	public int getListeningEvents() {
		return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
	}

	@Override
	public void serialEvent(SerialPortEvent event) {
		if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
			return;
		}

		int sum = 0;

		for (byte oneByte : model.receiveBytes()) {
			sum *= 10;
			sum += oneByte - 48;
		}

		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			throw new RuntimeException(e);
		}
		final int finalSum = sum;
		Platform.runLater(() -> model.setGuesses(finalSum));
		Platform.runLater(this::updateView);
	}
}
