#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// Structure - added 'paid' and 'transaction_id' fields
struct Customer {
 int id;
 char name[30];
 char mobile[15];
 float units;
 float rate;
 int paid; // Added: 0 = unpaid, 1 = paid
 int transaction_id; // Added: store txn id per customer
};
// QR Code Simulation
void showQRCode() {
 printf("\nScan this QR (Simulation):\n\n");
 printf("\n");
printf("██████ ██ ██████ ██████\n");
printf("██ ██ ████ ██ ██ ██\n");
printf("██ ██ ██ ██ ██ ██ ██\n");
printf("██████ ██ ██████ ██████\n");
printf(" ██ ████ ██ ████ ██ \n");
printf("████ ██ ██████ ██ ████\n");
printf("██ ████ ██ ██ ████ ██\n");
printf("████ ██ ██████ ██ ████\n");
printf("██████ ██ ██████ ██████\n");
printf("██ ██ ████ ██ ██ ██\n");
printf("██ ██ ██ ██ ██ ██ ██\n");
printf("██████ ██ ██████ ██████\n"); 
}
// SMS Simulation
void sendSMS(char mobile[], float amount) {
 printf("\nSending SMS to %s...\n", mobile);
 printf("Message:\n");
 printf("Your electricity bill of Rs. %.2f has been paid successfully.\n", amount);
 printf("Thank you! - Electricity Board\n");
}
int main() {
 struct Customer customers[] = {
 {1001, "Rahul Sharma", "9876543210", 250, 6.50, 0, 0},
 {1002, "Priya Verma", "9123456780", 320, 7.00, 0, 0},
 {1003, "Amit Kumar", "9988776655", 150, 5.75, 0, 0}
 };
 int id, choice;
 float amount, gst, total, penalty = 0, discount = 0;
 const float GST_RATE = 0.18;
 const int due_day = 5;
 const int penalty_per_day = 10;
 int again = 1; // Added: to repeat for multiple customers
 srand(time(NULL)); // Added: seed for random transaction ID
 while (again == 1) {
 penalty = 0; // Reset
 discount = 0; // Reset
 // Date
 time_t t = time(NULL);
 struct tm today = *localtime(&t);
 int current_day = today.tm_mday;
 printf("\nToday's Date: %02d-%02d-%d\n",
 today.tm_mday, today.tm_mon + 1, today.tm_year + 1900);
 // Input ID
 printf("Enter Customer ID: ");
 scanf("%d", &id);
 // Find customer
 int found = -1;
 for (int i = 0; i < 3; i++) {
 if (customers[i].id == id) {
 found = i;
 break;
 }
 }
 if (found == -1) {
 printf("Invalid Customer ID\n");
 } else {
 // Use pointer so we can update paid status directly
 struct Customer *c = &customers[found];
 // Show immediately if already paid
 if (c->paid == 1) {
 printf("\nNOTE: Bill for %s is already PAID.\n", c->name);
 printf("Transaction ID: TXN%d\n", c->transaction_id);
 }
 // Calculations
 amount = c->units * c->rate;
 gst = amount * GST_RATE;
 // Penalty
 if (current_day > due_day) {
 penalty = (current_day - due_day) * penalty_per_day;
 }
 // Discount (early payment)
 if (current_day <= due_day) {
 discount = 0.05 * amount;
 }
 total = amount + gst + penalty - discount;
 // Menu
 do {
 printf("\n===== MENU for %s =====\n", c->name);
 printf("1. View Bill\n");
 printf("2. Pay Bill (UPI)\n");
 printf("3. Exit Customer\n");
 printf("Enter choice: ");
 scanf("%d", &choice);
 switch (choice) {
 case 1:
 printf("\n----- ELECTRICITY BILL -----\n");
 printf("Customer ID : %d\n", c->id);
 printf("Customer Name : %s\n", c->name);
 printf("Mobile Number : %s\n", c->mobile);
 printf("Units Consumed : %.2f\n", c->units);
 printf("Rate per Unit : %.2f\n", c->rate);
 printf("Base Amount : %.2f\n", amount);
 printf("GST (18%%) : %.2f\n", gst);
 printf("Penalty : %.2f\n", penalty);
 printf("Discount (5%%) : -%.2f\n", discount);
 printf("Total Amount : %.2f\n", total);
 if (c->paid)
 printf("Status : PAID - TXN%d\n", c->transaction_id);
 else
 printf("Status : PENDING\n");
 break;
 case 2:
 if (c->paid) {
 printf("Bill already paid. Transaction ID: TXN%d\n", c->transaction_id);
 } else {
 printf("\n----- UPI PAYMENT -----\n");
 printf("UPI Link:\n");
 
printf("upi://pay?pa=demo@upi&pn=ElectricityBoard&am=%.2f&cu=INR\n", total);
 showQRCode();
 printf("Press 1 after payment: ");
 int confirm;
 scanf("%d", &confirm);
 if (confirm == 1) {
 c->paid = 1; // Update customer struct directly
 // Generate Transaction ID
 c->transaction_id = rand() % 90000 + 10000;
 printf("\nPayment Successful!\n");
 printf("Transaction ID: TXN%d\n", c->transaction_id);
 // Save receipt
 FILE *fp = fopen("receipt.txt", "w");
 if (fp!= NULL) {
 fprintf(fp, "----- ELECTRICITY BILL RECEIPT -----\n");
 fprintf(fp, "Customer Name : %s\n", c->name);
 fprintf(fp, "Mobile : %s\n", c->mobile);
 fprintf(fp, "Units : %.2f\n", c->units);
 fprintf(fp, "Amount : %.2f\n", amount);
 fprintf(fp, "GST : %.2f\n", gst);
 fprintf(fp, "Penalty : %.2f\n", penalty);
 fprintf(fp, "Discount : %.2f\n", discount);
 fprintf(fp, "Total Paid : %.2f\n", total);
 fprintf(fp, "TransactionID : TXN%d\n", c->transaction_id);
 fprintf(fp, "Status : PAID\n");
 fclose(fp);
 printf("Receipt saved to receipt.txt\n");
 } else {
 printf("Error saving receipt!\n");
 }
 // SMS
 sendSMS(c->mobile, total);
 } else {
 printf("Payment Failed or Cancelled\n");
 }
 }
 break;
 case 3:
 printf("Exiting customer %d...\n", c->id);
 break;
 default:
 printf("Invalid choice\n");
 }
 } while (choice!= 3);
 }
 printf("\nProcess another customer? 1 = Yes, 0 = No: ");
 scanf("%d", &again);
 }
 printf("Program ended.\n");
 return 0;
}