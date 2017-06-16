
public class checaLinhasEColunas extends Thread {
	private int k,l;
	private int TL,TC;

	public checaLinhasEColunas(int k, int l, int threadsLinha, int threadsColuna) {
		// TODO Auto-generated constructor stub
		this.k = k;
		this.l = l;
		TL = threadsLinha;
		TC = threadsColuna;
	}

	public void run() {

		int restoLinha = 9 % TL;
		int restoColuna = 9 % TC;

		int threadLines = 9 / TL;
		int threadColumns = 9 / TC;
		
		main.organiza(k,l,restoLinha,restoColuna,threadLines,threadColumns);
	}
	
	
	




}
