#ifndef cores_hpp
#define cores_hpp

// class with static members/methods to 
// facilitate setting/retrieving number of cores
class CoresInformation
{	
	private : 
		static int coresNo;

	public : 
		static void setCores(int cores) { coresNo = cores;}

		static int getCores() { return coresNo; }
};

#endif