import javax.swing.JOptionPane;

public class main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		numeroDeThreads = Runtime.getRuntime().availableProcessors();
		if (args.length >= 1) {
			nomeArquivo = args[0];
			if (args.length >= 2)
				numeroDeThreads = Integer.parseInt(args[args.length - 1]);
		} else {
			nomeArquivo = JOptionPane.showInputDialog("Digite o nome do Arquivo sudoku");
		}
		if (numeroDeThreads > 81)
			numeroDeThreads = 81;

		int temp = (int) Math.sqrt(numeroDeThreads);
		int temp2 = temp;
		if (numeroDeThreads % temp != 0)
			for (int i = 1; i < temp; i++)
				if (numeroDeThreads % i == 0)
					temp2 = i;
		threadsColuna = temp2;
		threadsLinha = numeroDeThreads / temp2;
		

		System.out.printf("Numero de threads usadas : %d\nNumero de Threads Linha : %d\nNumero de Threads Coluna : %d\n", numeroDeThreads,threadsLinha,threadsColuna);

		LeitorSudoku leitor = new LeitorSudoku(nomeArquivo);
		campoSudoku = leitor.getNumeros();

		criaThreadsLinhaColuna();

		printer();
	}

	private static void criaThreadsLinhaColuna() {
		int i, j;
		for (i = 0; i < 9; i++) {
			errosSudoku[0][i] = 0;
			errosSudoku[1][i] = 0;
			errosSudoku[2][i] = 0;
		}
		checaLinhasEColunas[][] CLC = new checaLinhasEColunas[threadsLinha][threadsColuna];
		for (i = 0; i < threadsLinha; i++)
			for (j = 0; j < threadsColuna; j++) {
				CLC[i][j] = new checaLinhasEColunas(i, j, threadsLinha, threadsColuna);
				CLC[i][j].start();
			}
		for (i = 0; i < threadsLinha; i++)
			for (j = 0; j < threadsColuna; j++)
				try {
					CLC[i][j].join();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}

		checaRegioes CR[] = new checaRegioes[threadsColuna];
		for (i = 0; i < threadsColuna; i++) {
			CR[i] = new checaRegioes(i, threadsColuna, errosSudoku, campoSudoku);
			CR[i].start();
		}
		for (i = 0; i < threadsColuna; i++)
			try {
				CR[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
	}

	private static void printer() {

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 9; j++) {
				if (i == 0)
					if (errosSudoku[i][j] == 1) {
						System.out.printf("Erro na linha	%d\n", j + 1);
						mostrouErros = 1;
					}
				if (i == 1)
					if (errosSudoku[i][j] == 1) {
						System.out.printf("Erro na coluna	%d\n", j + 1);
						mostrouErros = 1;
					}
				if (i == 2)
					if (errosSudoku[i][j] == 1) {
						System.out.printf("Erro na região	%d\n", j + 1);
						mostrouErros = 1;
					}
			}
		if (mostrouErros == 0) {
			System.out.println("Sem erros");
		}
	}
	
	public static void organiza(int k, int l, int restoLinha, int restoColuna, int threadLines, int threadColumns) {
		int i, j, m;
		int threadLEnd = k == threadsLinha-1? ((k+1)*threadLines)+restoLinha : (k+1)*threadLines;
		int threadCEnd = l == threadsColuna-1? ((k+1)*threadColumns)+restoColuna : (k+1)*threadColumns;
		for (i = k * threadLines; i < threadLEnd; i++) {
			for (j = l * threadColumns; j < threadCEnd; j++) {
				for (m = j + 1; m < 9; m++) {
					checaLinha(i, j, m);
					checaColuna(i, j, m);
				}
			}
		}
	}

	public static synchronized void checaLinha(int i, int j, int k) {
		if (errosSudoku[0][i] == 0)
			if (campoSudoku[i][j] == campoSudoku[i][k])
				errosSudoku[0][i] = 1;
	}

	public static synchronized void checaColuna(int i, int j, int k) {
		if (errosSudoku[1][i] == 0)
			if (campoSudoku[j][i] == campoSudoku[k][i])
				errosSudoku[1][i] = 1;
	}

	

	private static int[][] errosSudoku = new int[3][9], campoSudoku;
	private static String nomeArquivo;
	private static int numeroDeThreads, threadsLinha, threadsColuna, mostrouErros = 0;

}
