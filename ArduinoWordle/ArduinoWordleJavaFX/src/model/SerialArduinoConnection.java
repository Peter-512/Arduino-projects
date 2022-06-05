package model;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;

import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Locale;
import java.util.Random;

public class SerialArduinoConnection {
	private static final String[] wordChoices = { "able", "aces", "acid", "aged", "also", "area", "army", "away", "baby", "back", "ball", "band", "bank", "base", "bath", "bear", "beat", "been", "beer", "bell", "belt", "best", "bill", "bird", "blow", "blue", "boat", "body", "bomb", "bond", "bone", "book", "boom", "born", "boss", "both", "bowl", "bulk", "burn", "bush", "busy", "call", "calm", "came", "camp", "card", "care", "cats", "case", "cash", "cast", "cell", "chat", "chip", "city", "club", "coal", "coat", "code", "cold", "come", "cook", "cool", "cope", "copy", "CORE", "cost", "crew", "crop", "dark", "data", "date", "dawn", "days", "dead", "deal", "dean", "dear", "debt", "deep", "deny", "desk", "dial", "dick", "diet", "disc", "disk", "does", "done", "door", "dose", "down", "draw", "drew", "drop", "drug", "dual", "duke", "dust", "duty", "each", "earn", "ease", "east", "easy", "edge", "else", "even", "ever", "evil", "exit", "face", "fact", "fail", "fair", "fall", "farm", "fast", "fate", "fear", "feed", "feel", "feet", "fell", "felt", "file", "fill", "film", "find", "fine", "fire", "firm", "fish", "five", "flat", "flow", "food", "foot", "ford", "form", "fort", "four", "free", "from", "fuel", "full", "fund", "gain", "game", "gate", "gave", "gear", "gene", "gift", "girl", "give", "glad", "goal", "goes", "gold", "Golf", "gone", "good", "gray", "grew", "grey", "grow", "gulf", "hair", "half", "hall", "hand", "hang", "hard", "harm", "hate", "have", "head", "hear", "heat", "held", "hell", "help", "here", "hero", "high", "hill", "hire", "hold", "hole", "holy", "home", "hope", "host", "hour", "huge", "hung", "hunt", "hurt", "idea", "inch", "into", "iron", "item", "jack", "jane", "jean", "john", "join", "jump", "jury", "just", "keen", "keep", "kent", "kept", "kick", "kill", "kind", "king", "knee", "knew", "know", "lack", "lady", "laid", "lake", "land", "lane", "last", "late", "lead", "left", "less", "life", "lift", "like", "line", "link", "list", "live", "load", "loan", "lock", "logo", "long", "look", "lord", "lose", "loss", "lost", "love", "luck", "made", "mail", "main", "make", "male", "many", "Mark", "mass", "matt", "meal", "mean", "meat", "meet", "menu", "mere", "mike", "mile", "milk", "mill", "mind", "mine", "miss", "mode", "mood", "moon", "more", "most", "move", "much", "must", "name", "navy", "near", "neck", "need", "news", "next", "nice", "nick", "nine", "none", "nose", "note", "okay", "once", "only", "onto", "open", "oral", "over", "pace", "pack", "page", "paid", "pain", "pair", "palm", "park", "part", "pass", "past", "path", "peak", "pick", "pink", "pipe", "plan", "play", "plot", "plug", "plus", "poll", "pool", "poor", "port", "post", "pull", "pure", "push", "race", "rail", "rain", "rank", "rare", "rate", "read", "real", "rear", "rely", "rent", "rest", "rice", "rich", "ride", "ring", "rise", "risk", "road", "rock", "role", "roll", "roof", "room", "root", "rose", "rule", "rush", "ruth", "safe", "said", "sake", "sale", "salt", "same", "sand", "save", "seat", "seed", "seek", "seem", "seen", "self", "sell", "send", "sent", "sept", "ship", "shop", "shot", "show", "shut", "sick", "side", "sign", "site", "size", "skin", "slip", "slow", "snow", "soft", "soil", "sold", "sole", "some", "song", "soon", "sort", "soul", "spot", "star", "stay", "step", "stop", "such", "suit", "sure", "take", "tale", "talk", "tall", "tank", "tape", "task", "team", "tech", "tell", "tend", "term", "test", "text", "than", "that", "them", "then", "they", "thin", "this", "thus", "till", "time", "tiny", "told", "toll", "tone", "tony", "took", "tool", "tour", "town", "tree", "trip", "true", "tune", "turn", "twin", "type", "unit", "upon", "used", "user", "vary", "vast", "very", "vice", "view", "vote", "wage", "wait", "wake", "walk", "wall", "want", "ward", "warm", "wash", "wave", "ways", "weak", "wear", "week", "well", "went", "were", "west", "what", "when", "whom", "wide", "wife", "wild", "will", "wind", "wine", "wing", "wire", "wise", "wish", "with", "wood", "word", "wore", "work", "yard", "yeah", "year", "your", "zero", "zone"};
	private final SerialPort arduinoPort;
	private String secret;
	private String guess;
	private int guesses;

	public SerialArduinoConnection() {
		arduinoPort = (SerialPort) Arrays.stream(SerialPort.getCommPorts())
		                                 .filter(serialPort -> serialPort.toString().contains("USB") && serialPort.toString().contains("(Dial-In)"))
		                                 .toArray()[0];
		boolean result = arduinoPort.openPort();
		System.out.printf("Port %s%s opened!\n", arduinoPort, result ? "" : " NOT");
		secret = getRandomWord();
		guess = "";
	}

	public void sendString(String data) {
//		System.out.printf("Sending secret word %s to Arduino...\n", data); // ! DEBUG ONLY, REVEALS THE SECRET WORD
		PrintWriter writer = new PrintWriter(arduinoPort.getOutputStream());
		writer.print(data);
		writer.close();
	}

	public byte[] receiveBytes() {
		byte[] newData = new byte[arduinoPort.bytesAvailable()];
		arduinoPort.readBytes(newData, newData.length);
		return newData;
	}

	public boolean isPossibleWord(String word) {
		return Arrays.asList(wordChoices).contains(word.toLowerCase(Locale.ROOT));
	}

	private String getRandomWord() {
		Random random = new Random();
		return wordChoices[random.nextInt(wordChoices.length)].toUpperCase(Locale.ROOT);
	}

	public void addDataListener(SerialPortDataListener dataListener) {
		arduinoPort.addDataListener(dataListener);
	}

	public void removeDataListener() {
		arduinoPort.removeDataListener();
	}

	public String getSecret() {
		return secret;
	}

	public void setSecret() {
		secret = getRandomWord();
	}

	public boolean isLetterInSecret(char letter) {
		return secret.indexOf(letter) != -1;
	}

	public void setGuess(String guess) {
		this.guess = guess;
	}

	public boolean isGuessCorrect() {
		return secret.equals(guess);
	}

	public String getGuess() {
		return guess;
	}

	public char[] getGuessArray() {
		return guess.toCharArray();
	}

	public int getGuesses() {
		return guesses;
	}

	public void setGuesses(int guesses) {
		this.guesses = guesses;
	}
}
