#ifndef cores_hpp
#define cores_hpp

class CoresInformation
{	
	private : 
		static int coresNo;

	public : 
		static void setCores(int cores) { coresNo = cores;}

		static int getCores() { return coresNo; }
};

#endif