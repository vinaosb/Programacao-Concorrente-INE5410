import java.io.*;

public class LeitorSudoku {

	public LeitorSudoku(String nomeArquivo) {
		// TODO Auto-generated constructor stub
		try {
			leitor = new BufferedReader(new FileReader(nomeArquivo));
			linhas = new String[9];
			for (int i = 0; i < 9; i++) {
				linhas[i] = leitor.readLine();
			}
			int k;
			for (int i = 0; i < 9; i++){
				k = 0;
				for (int j = 0; j < 17; j++) {
					if (Character.getNumericValue(linhas[i].charAt(j)) != -1) {
						numeros[i][k] = Character.getNumericValue(linhas[i].charAt(j));
						k++;
					}
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public int[][] getNumeros() {
		return numeros;
	}

	private String[] linhas;
	private int[][] numeros = new int[9][9];
	private BufferedReader leitor;

}
