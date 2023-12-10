defmodule Solution do
  def select(list, indices) do
    Enum.map(indices, &(Enum.at(list, &1)))
  end
  
  def is_digit(char) when is_binary(char) do
    case Integer.parse(char) do
      {_, _} -> true
      _ -> false
    end
  end
  
  def part1(c) do
    c |> String.split("\n", trim: true)
      |> Enum.map( fn s -> s
            |> String.graphemes()
            |> Enum.filter(&is_digit/1)
            |> select([1, -1])
            |> Enum.join()
            |> String.to_integer()
          end)
      |> Enum.sum()
      |> IO.inspect()
  end

  def first_digit(str) do
    cond do
      (String.split(str, "1") |> Enum.count()) == 2 -> 1
      (String.split(str, "2") |> Enum.count()) == 2 -> 2
      (String.split(str, "3") |> Enum.count()) == 2 -> 3
      (String.split(str, "4") |> Enum.count()) == 2 -> 4
      (String.split(str, "5") |> Enum.count()) == 2 -> 5
      (String.split(str, "6") |> Enum.count()) == 2 -> 6
      (String.split(str, "7") |> Enum.count()) == 2 -> 7
      (String.split(str, "8") |> Enum.count()) == 2 -> 8
      (String.split(str, "9") |> Enum.count()) == 2 -> 9
      true -> :error
    end
  end

  def part2(c) do
    c |> String.split("\n", trim: true)
      |> Enum.map( &first_digit/1 )
      |> IO.inspect()
  end
end

case File.read "example.txt" do
  {:ok, c} -> 
    Solution.part2(c)
  {:error, _} -> IO.puts "Failed to read .txt"
end
