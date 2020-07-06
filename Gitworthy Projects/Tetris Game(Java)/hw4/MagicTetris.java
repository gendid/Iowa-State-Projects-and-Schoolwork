
package hw4;

import java.awt.Color;
import java.util.ArrayList;
import java.util.List;

import api.AbstractGame;
import api.Position;

/**
 * MagicTetris implementation.
 * 
 * @author Gendid Will Springer
 */
public class MagicTetris extends AbstractGame
{
	/**
	 * Determines if gravity mode is on
	 */
	private boolean				gravity;
	/**
	 * Keeps track of the magic blocks, for use for score and gravity mode
	 */
	private int					magicCounter;
	/**
	 * Keeps the score of the game
	 */
	private int					score;
	/**
	 * Constant used in the score calculations
	 */
	private final static double	POINT	= 2;

	/**
	 * Constructs a game with the given width (columns) and height (rows). This game
	 * will use a new instance of BasicGenerator to generate new shapes.
	 * 
	 * @param width
	 *            width of the game grid (number of columns)
	 * @param height
	 *            height of the game grid (number of rows)
	 */
	public MagicTetris(int width, int height) {
		super(width, height, new BasicGenerator());
		gravity = false;
		score = 0;
		magicCounter = 0;
	}

	/**
	 * Determines which blocks to collapse. Changes function based on if gravity is
	 * active or not. Also updates the score upon a successful row completion.
	 */
	@Override
	public List<Position> determinePositionsToCollapse()
		{

			int currentCols = super.getHeight();
			int currentRows = super.getWidth();
			ArrayList<Position> collapseList = new ArrayList<>();
			ArrayList<Position> noMagicList = new ArrayList<>();
			ArrayList<Position> magicList = new ArrayList<>();
			for (int i = 0; i < super.getHeight(); i++) {
				for (int j = 0; j < super.getWidth(); j++) {
					if (super.getBlock(i, j) != null) {
						collapseList.add(new Position(i, j));
					}
				}

			}
			int blockCounter = 0;

			for (int i = 0; i < currentCols; i++) {
				for (int j = 0; j < currentRows; j++) {
					if (collapseList.contains(new Position(i, j))) {
						blockCounter++;
					}
					if (j == currentRows - 1) {
						if (blockCounter == currentRows) {
							for (int z = currentRows - 1; z >= 0; z--) {
								noMagicList.add(new Position(i, z));
								if (super.getBlock(i, z).isMagic()) {
									magicCounter++;
								}
							}
						}
						blockCounter = 0;
					}
				}
			}
			if (gravity == true) {
				for (int i = 0; i < collapseList.size(); i++) {
					for (int j = collapseList.get(i).row(); j < currentCols; j++) {
						if (super.getBlock(j, collapseList.get(i).col()) == null) {
							magicList.add(new Position(j, collapseList.get(i).col()));
						}
					}
				}

				gravity = false;
				return magicList;
			}
			if (magicCounter >= 3) {
				gravity = true;
			}
			if (!noMagicList.isEmpty()) {
				score += (int) Math.pow(POINT, magicCounter);
			}
			magicCounter = 0;
			return noMagicList;
		}

	/**
	 * Returns the current score value
	 */
	@Override
	public int determineScore()
		{

			return score;
		}

}
