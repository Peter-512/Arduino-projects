package view.wordle;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import model.SerialArduinoConnection;
import view.endscreen.EndScreenPresenter;
import view.endscreen.EndScreenView;

public class WordlePresenter implements SerialPortDataListener {
	private final SerialArduinoConnection model;
	private final WordleView view;

	public WordlePresenter(SerialArduinoConnection model, WordleView view) {
		this.model = model;
		this.view = view;
		model.addDataListener(this);
		addEventHandlers();
	}

	private void addEventHandlers() {

	}

	private void updateView() {
		if (model.isPossibleWord(model.getGuess())) {
			view.getText().setText(null);
			for (int i = 0; i < model.getGuessArray().length || i < view.getHints().getChildren().size(); i++) {
				char letter = model.getGuessArray()[i];
				final Rectangle rectangle = (Rectangle) view.getHints().getChildren().get(i);

				if (!model.isLetterInSecret(letter)) {
					rectangle.setFill(Color.BLACK);
				} else if (letter == model.getSecret().toCharArray()[i]) {
					rectangle.setFill(Color.GREEN);
				} else {
					rectangle.setFill(Color.YELLOW);
				}
			}
		} else {
			view.getText().setText(String.format("%s is not a word!", model.getGuess()));
			for (int i = 0; i < model.getGuessArray().length; i++) {
				((Rectangle) view.getHints().getChildren().get(i)).setFill(Color.BLACK);
			}
		}
		if (model.isGuessCorrect()) {
			System.out.println("Guessed correctly!");
			model.sendString("You win!");

			//			Set EndScreen
			model.removeDataListener();
			final EndScreenView endScreenView = new EndScreenView();
			new EndScreenPresenter(model, endScreenView);
			view.getScene().setRoot(endScreenView);
			Stage stage = (Stage) endScreenView.getScene().getWindow();
			endScreenView.getScene().getWindow().sizeToScene();
			stage.centerOnScreen();
		}
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
		String guess = model.getGuess();
		StringBuilder text;
		if (guess.length() >= 4) {
			text = new StringBuilder();
		} else {
			text = new StringBuilder(guess);
		}

		for (byte oneByte : model.receiveBytes()) {
			text.append((char) oneByte);
		}
		Platform.runLater(() -> model.setGuess(text.toString()));
		Platform.runLater(this::updateView);
	}
}
