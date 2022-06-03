package view.endscreen;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.layout.BorderPane;
import javafx.scene.text.Font;
import javafx.scene.text.Text;

public class EndScreenView extends BorderPane {
	private Text text;
	private Text guesses;
	private Button closeGame;

	public EndScreenView() {
		initializeNodes();
		layoutNodes();
	}

	private void initializeNodes() {
		text = new Text("You won!");
		guesses = new Text();
		closeGame = new Button("Close game");
	}

	private void layoutNodes() {
		text.setFont(new Font(70));
		setAlignment(text, Pos.CENTER);
		setMargin(text, new Insets(20));
		setTop(text);

		guesses.setFont(new Font(50));
		setAlignment(guesses, Pos.CENTER);
		setMargin(guesses, new Insets(20));
		setCenter(guesses);

		closeGame.setFont(new Font(20));
		setAlignment(closeGame, Pos.CENTER);
		setMargin(closeGame, new Insets(20));
		setBottom(closeGame);
	}

	Text getGuesses() {
		return guesses;
	}

	Button getCloseGame() {
		return closeGame;
	}
}
