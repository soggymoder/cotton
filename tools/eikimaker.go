// i would have made this as a shell script but im more familiar with Go anyway so blehh
package main

import (
	"fmt"
	"os"
)

func main() {

	fmt.Print("da character / letter : ")

	var ch string
	fmt.Scanln(&ch)

	file, err := os.OpenFile("eikis.txt", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)

	if err != nil {
		fmt.Println("couldn't open eikis.txt :(")
		return
	}

	defer file.Close()

	fmt.Println("type a # for a pixel and a . for an empty spot")

	rows := make([]string, 8)

	for i := 0; i < 8; i++ {
		fmt.Printf("%d: ", i)
		fmt.Scanln(&rows[i])
	}

	fmt.Fprintln(file)
	fmt.Fprintf(file, "['%s'] = {\n", ch)

	for _, row := range rows {
		var value uint8

		for i, pixel := range row {
			if i >= 6 {
				break
			}

			if pixel == '#' {
				value |= 1 << i
			}
		}

		fmt.Fprintf(file, "    0b%08b,\n", value)
	}

	fmt.Fprintln(file, "},")

}
