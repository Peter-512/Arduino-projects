package view.mainmenu;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;
import javafx.stage.Stage;
import model.SerialArduinoConnection;
import view.wordle.WordlePresenter;
import view.wordle.WordleView;

public class MainMenuPresenter implements SerialPortDataListener {
	private final SerialArduinoConnection model;
	private final MainMenuView view;

	public MainMenuPresenter(SerialArduinoConnection model, MainMenuView view) {
		this.model = model;
		this.view = view;
		addEventListeners();
		model.addDataListener(this);
	}

	private void addEventListeners() {
		view.getStartButton().setOnAction(actionEvent -> {
			model.sendString(model.getSecret());
			model.removeDataListener();
			final WordleView wordleView = new WordleView();
			new WordlePresenter(model, wordleView);
			view.getScene().setRoot(wordleView);
			Stage stage = (Stage) wordleView.getScene().getWindow();
			wordleView.getScene().getWindow().sizeToScene();
			stage.centerOnScreen();
		});
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
		StringBuilder text = new StringBuilder();
		for (byte oneByte : model.receiveBytes()) {
			text.append((char) oneByte);
		}
		Platform.runLater(() -> System.out.println(text));
	}
}
