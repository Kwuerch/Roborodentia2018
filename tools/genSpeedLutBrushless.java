public class genSpeedLutBrushless{
   private static final int LOW_FREQ = 17637;
   private static final int HIGH_FREQ =32066;
   private static final int LUT_SIZE = 256;
   private static final double BASE_FREQ = 320664;

   public static void main(String[] args){
      float div_factor = (HIGH_FREQ - LOW_FREQ)/LUT_SIZE;
      System.out.println(div_factor);

      for(int i = 0; i < LUT_SIZE; i++){
         System.out.print((int)(LOW_FREQ +(i*div_factor)));

         if(i % 8 != 7){
            System.out.print(",");
         }else{
            System.out.println(",");
         }
      }
   }
}
