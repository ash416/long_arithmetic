import java.io.FileWriter;
import java.io.IOException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.DecimalFormat;

public class DoubleGenerator {
    public static void main(String[] args) throws IOException {
       // generateSum("sum_double.txt");
        //generateDiff("diff_double1.txt");
        //generateMult("mult_double2.txt");
        generateDiv("div_double.txt");
    }

    private static void generateDiv(String file_name) throws IOException {
        FileWriter nFile = new FileWriter("C:/Users/ash41/Documents/Visual Studio 2015/Projects/LongInteger/LongInteger/data/" + file_name);
        for (int i = 0; i < 100; i++){
            BigDecimal num1 = generateDouble();
            BigDecimal num2 = generateDouble();
            if (num1.compareTo(num2) < 0) {
                i--;
                continue;
            }
            BigDecimal sum = num1.divide(num2, 10, RoundingMode.DOWN);
            nFile.write(num1.toPlainString() + ' ' + num2.toPlainString() + ' ' + sum.toPlainString() + '\n');
        }
        nFile.close();
    }

    private static void generateMult(String file_name) throws IOException {
        FileWriter nFile = new FileWriter("C:/Users/ash41/Documents/Visual Studio 2015/Projects/LongInteger/LongInteger/data/" + file_name);
        for (int i = 0; i < 100; i++){
            BigDecimal num1 = generateDouble();
            BigDecimal num2 = generateDouble();
            BigDecimal sum = num1.multiply(num2);
            nFile.write(num1.toPlainString() + ' ' + num2.toPlainString() + ' ' + sum.toPlainString() + '\n');
        }
        nFile.close();
    }

    private static void generateSum(String file_name) throws IOException {
        FileWriter nFile = new FileWriter("C:/Users/ash41/Documents/Visual Studio 2015/Projects/LongInteger/LongInteger/data/" + file_name);
        for (int i = 0; i < 100; i++){
            BigDecimal num1 = generateDouble();
            BigDecimal num2 = generateDouble();
            BigDecimal sum = num1.add(num2);
            nFile.write(num1.toPlainString() + ' ' + num2.toPlainString() + ' ' + sum.toPlainString() + '\n');
        }
        nFile.close();
    }

    private static void generateDiff(String file_name) throws IOException {
        FileWriter nFile = new FileWriter("C:/Users/ash41/Documents/Visual Studio 2015/Projects/LongInteger/LongInteger/data/" + file_name);
        for (int i = 0; i < 10; i++){
            BigDecimal num1 = generateDouble();
            BigDecimal num2 = generateDouble();
            BigDecimal sum = num1.subtract(num2);
            nFile.write(num1.toPlainString() + ' ' + num2.toPlainString() + ' ' + sum.toPlainString() + '\n');
        }
        nFile.close();
    }

    private static BigDecimal generateDouble(){
        BigDecimal a, b;
        do {
            a = getRandNum(rnd(10, 1000));
            b = getRandNum(rnd(10, 100));
        } while (a.compareTo(b) < 0);
        return a.divide(b, rnd(1,10), RoundingMode.HALF_UP);
    }
    private static int rnd(int min, int max){
        max -=min;
        return (int) (Math.random() * ++max) + min;
    }

    private static BigDecimal getRandNum(int size){
        String num = "";
        for(int i = 0; i < size; i++){
            int a = rnd(0, 9);
            if (a == 0 && i == 0) {
                i--;
                continue;
            }
            num = num + Integer.toString(a);
        }
        return new BigDecimal(num);
    }
}
