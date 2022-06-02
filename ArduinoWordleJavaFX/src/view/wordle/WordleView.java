package view.wordle;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;

public class WordleView extends BorderPane {
	static final int MAX_LETTERS = 4;
	private Text colorLegend;
	VBox top;
	Text text;
	private Text instructions;
	private HBox hints;

	public WordleView() {
		initializeNodes();
		layoutNodes();
	}

	private void initializeNodes() {
		hints = new HBox();
		for (int i = 0; i < MAX_LETTERS; i++) {
			Rectangle letter = new Rectangle(50, 100, Color.BLACK);
			hints.getChildren().add(letter);
		}

		instructions = new Text("""
				Left button:   Move one letter to the left.
				Right button:  Move one letter to the right.
				Center button: Confirm guess.
				Potentiometer: Choosing letter.
				You have to guess real words.""");

		colorLegend = new Text("""
				Black:  Letter is not in the word.
				Yellow: Letter is in the word but not in the correct position.
				Green:  Letter is in the correct position.""");

		text = new Text();
		top = new VBox(instructions, text);
	}

	private void layoutNodes() {
		instructions.setFont(new Font("Monospaced Regular", 14));
		top.setAlignment(Pos.CENTER);
		setMargin(top, new Insets(10));
		setTop(top);

		for (Node child : hints.getChildren()) {
			Rectangle letter = (Rectangle) child;
			letter.setArcHeight(10);
			letter.setArcWidth(10);
		}
		setAlignment(hints, Pos.CENTER);
		hints.setAlignment(Pos.CENTER);
		hints.setSpacing(20);
		setMargin(hints, new Insets(20));
		setCenter(hints);

		colorLegend.setFont(new Font("Monospaced Regular", 14));
		setAlignment(colorLegend, Pos.CENTER);
		setMargin(colorLegend, new Insets(10));
		setBottom(colorLegend);
	}

	public HBox getHints() {
		return hints;
	}

	public Text getText() {
		return text;
	}
}
