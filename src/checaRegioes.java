

public class checaRegioes extends Thread {
	private int k, TC;
	private static int errosSudoku[][], campoSudoku[][];
	private static int MC[][][];

	public checaRegioes(int k, int threadsColuna, int[][] ES, int[][] campoSudoku) {
		// TODO Auto-generated constructor stub
		this.k = k;
		TC = threadsColuna;
		errosSudoku = ES;
		MC = new int[9][3][3];
		checaRegioes.campoSudoku = campoSudoku;
	}

	public void run() {
		int i;
		int restoColuna = 9 % TC;
		int threadColumns = 9 / TC;
		criaMiniCampos();

		for (i = k * threadColumns; i < (k + 1) * threadColumns + restoColuna; i++) {
			if (errosSudoku[2][i] == 0)
				checaRegiao(i);
		}

	}

	private static void criaMiniCampos() {
		int i, j, n;
		for (i = 0; i < 9; i++) {
			n = 6;
			if (i < 6)
				n = 3;
			if (i < 3)
				n = 0;

			for (j = 0; j < 9; j++) {
				MC[n][i % 3][j % 3] = campoSudoku[i][j];
				if (j % 3 == 2)
					n++;
			}
		}
	}

	private static void checaRegiao(int i) {
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				for (int l = 0; l < 3; l++)
					for (int m = 0; m < 3; m++)
						if (l != j | m != k)
							if (MC[i][j][k] == MC[i][l][m]) {
								errosSudoku[2][i] = 1;
							}
	}

}
