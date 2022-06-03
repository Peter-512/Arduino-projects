package view.mainmenu;

import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.BorderPane;
import javafx.scene.text.Font;

public class MainMenuView extends BorderPane {
	private Button startButton;

	public MainMenuView() {
		initialiseNodes();
		layoutNodes();
	}

	private void initialiseNodes() {
		startButton = new Button("Start!");
		startButton.setFont(new Font(70));
		startButton.setDefaultButton(true);
	}

	private void layoutNodes() {
		setBottom(startButton);
		setPadding(new Insets(100));
	}

	Button getStartButton() {
		return startButton;
	}

}
