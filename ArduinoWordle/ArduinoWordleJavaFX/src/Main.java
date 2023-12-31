import model.SerialArduinoConnection;
import view.mainmenu.MainMenuPresenter;
import view.mainmenu.MainMenuView;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
    public static void main(String[] args) {
        Application.launch(args);
    }
    @Override
    public void start(Stage stage) {
        SerialArduinoConnection connection = new SerialArduinoConnection();
        MainMenuView view = new MainMenuView();
        new MainMenuPresenter(connection, view);
        stage.setScene(new Scene(view));
        stage.show();
    }
}
